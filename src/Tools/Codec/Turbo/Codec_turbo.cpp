#include <functional>
#include <stdexcept>
#include <cctype>

#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_encoder_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_puncturer_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_decoder_turbo.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp"

#include "Codec_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::Codec_turbo(const parameters& params)
: Codec<B,Q>(params),
  sub_enc  (this->params.simulation.n_threads, nullptr),
  siso     (this->params.simulation.n_threads, nullptr),
  post_pros(this->params.simulation.n_threads         )
{
	if (!params.simulation.json_path.empty())
	{
		if (this->params.simulation.n_threads != 1)
			throw std::runtime_error("aff3ct::tools::Codec_turbo: only single-threaded simulation is "
			                         "available with JSON trace.");

		json_stream.open(params.simulation.json_path.c_str(), std::ios::out | std::ios::trunc);

		json_stream << "[" << std::endl;
	}

	auto encoder_RSC = Factory_encoder_RSC<B>::build("RSC",
	                                                 this->params.code.K,
	                                                (this->params.code.K * 2) + (this->params.code.tail_length / 2),
	                                                 this->params.encoder.buffered,
	                                                 this->params.encoder.poly);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;
}

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::~Codec_turbo()
{
	int tid = 0;
	const int nthr = this->params.simulation.n_threads;
	for (tid = 0; tid < nthr; tid++) if (sub_enc[tid] != nullptr) { delete sub_enc[tid]; sub_enc[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso   [tid] != nullptr) { delete siso   [tid]; siso   [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) clear_post_processing(tid);

	if (json_stream.is_open())
	{
		json_stream << "[{\"stage\": \"end\"}]]" << std::endl;
		json_stream.close();
	}
}

template <typename B, typename Q, typename QD>
void Codec_turbo<B,Q,QD>
::clear_post_processing(const int tid)
{
	if (post_pros[tid].size())
		for (auto i = 0; i < (int)post_pros[tid].size(); i++)
			if (post_pros[tid][i] != nullptr)
			{
				delete post_pros[tid][i];
				post_pros[tid][i] = nullptr;
			}

	post_pros[tid].clear();
}

template <typename B, typename Q, typename QD>
Interleaver<int>* Codec_turbo<B,Q,QD>
::build_interleaver(const int tid, const int seed)
{
	return Factory_interleaver<int>::build(this->params.interleaver.type,
	                                       this->params.code.K,
	                                       this->params.interleaver.path,
	                                       this->params.interleaver.uniform,
	                                       this->params.interleaver.n_cols,
	                                       seed,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
Encoder_RSC_sys<B>* Codec_turbo<B,Q,QD>
::build_sub_encoder(const int tid)
{
	return Factory_encoder_RSC<B>::build(this->params.simulation.json_path.empty() ? "RSC" : "RSC_JSON",
	                                     this->params.code.K,
	                                     (this->params.code.K * 2) + (this->params.code.tail_length / 2),
	                                     this->params.encoder.buffered,
	                                     this->params.encoder.poly,
	                                     json_stream,
	                                     this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
Encoder<B>* Codec_turbo<B,Q,QD>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"itl\" should not be null.");

	if (sub_enc[tid] != nullptr)
	{
		delete sub_enc[tid];
		sub_enc[tid] = nullptr;
	}

	sub_enc[tid] = this->build_sub_encoder(tid);

	if (sub_enc[tid] == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"sub_enc\" can't be created.");

	return Factory_encoder_turbo<B>::build(this->params.encoder.type,
	                                       this->params.code.K,
	                                       this->params.code.N_code,
	                                       *itl,
	                                       sub_enc[tid],
	                                       sub_enc[tid],
	                                       this->params.encoder.buffered,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
Puncturer<B,Q>* Codec_turbo<B,Q,QD>
::build_puncturer(const int tid)
{
	const std::string type = (this->params.code.N == this->params.code.N_code) ? "NO" : "WANGLIU";
	return Factory_puncturer_turbo<B,Q>::build(type,
	                                           this->params.code.K,
	                                           this->params.code.N,
	                                           this->params.code.tail_length,
	                                           this->params.puncturer.pattern,
	                                           this->params.encoder.buffered,
	                                           this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
SISO<Q>* Codec_turbo<B,Q,QD>
::build_sub_siso(const int tid)
{
	return Factory_decoder_RSC<B,Q,QD>::build(this->params.decoder.type,
	                                          this->params.decoder.implem,
	                                          this->params.code.K,
	                                          trellis,
	                                          this->params.decoder.max,
	                                          this->params.decoder.simd_strategy,
	                                          this->params.encoder.buffered,
	                                          json_stream,
	                                          this->params.decoder.n_ite,
	                                          this->params.simulation.inter_frame_level);
}

template <typename B, typename Q, typename QD>
Decoder<B,Q>* Codec_turbo<B,Q,QD>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	if (itl == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"itl\" should not be null.");

	clear_post_processing(tid);

	if (siso[tid] != nullptr)
	{
		delete siso[tid];
		siso[tid] = nullptr;
	}

	siso[tid] = this->build_sub_siso(tid);

	if (siso[tid] == nullptr)
		throw std::runtime_error("aff3ct::tools::Codec_turbo: \"siso\" can't be created.");

	auto decoder = Factory_decoder_turbo<B,Q>::build("TURBO",
	                                                 typeid(B) == typeid(long long) ? "STD" : "FAST",
	                                                 this->params.code.K,
	                                                 this->params.code.N_code,
	                                                 this->params.decoder.n_ite,
	                                                 *itl,
	                                                 *siso[tid],
	                                                 *siso[tid],
	                                                 this->params.encoder.buffered);

	if (!this->params.decoder.scaling_factor.empty())
	{
		std::string sf_type = this->params.decoder.scaling_factor;
		float sf_cst = 0.f;
		if (std::isdigit(this->params.decoder.scaling_factor[0]))
		{
			sf_type = "CST";
			sf_cst  = std::stof(this->params.decoder.scaling_factor);
		}

		post_pros[tid].push_back(Factory_scaling_factor<B,Q>::build(sf_type,
		                                                            this->params.decoder.n_ite,
		                                                            sf_cst));
	}

	if (this->params.decoder.fnc)
	{
		if (crc != nullptr && crc->get_size() > 0)
			post_pros[tid].push_back(new tools::Flip_and_check<B,Q>(this->params.code.K,
			                                                        this->params.decoder.n_ite,
			                                                        *crc,
			                                                        2,
			                                                        this->params.decoder.fnc_q,
			                                                        this->params.decoder.fnc_ite_min,
			                                                        this->params.decoder.fnc_ite_max,
			                                                        this->params.decoder.fnc_ite_step,
			                                                        decoder->get_simd_inter_frame_level()));
		else
			throw std::runtime_error("aff3ct::tools::Codec_turbo: the Flip aNd Check requires a CRC.");
	}
	else if (crc != nullptr && crc->get_size() > 0)
	{
		post_pros[tid].push_back(new tools::CRC_checker<B,Q>(*crc,
		                                                     2,
		                                                     decoder->get_simd_inter_frame_level()));
	}

	if (this->params.decoder.self_corrected)
	{
		post_pros[tid].push_back(new tools::Self_corrected<B,Q>(this->params.code.K,
		                                                        this->params.decoder.n_ite,
		                                                        4,
		                                                        this->params.decoder.n_ite,
		                                                        decoder->get_simd_inter_frame_level()));
	}

	for (auto i = 0; i < (int)post_pros[tid].size(); i++)
	{
		if (post_pros[tid][i] != nullptr)
		{
			using namespace std::placeholders;

			auto pp = post_pros[tid][i];
			decoder->add_handler_siso_n(std::bind(&Post_processing_SISO<B,Q>::siso_n, pp, _1, _2, _3, _4));
			decoder->add_handler_siso_i(std::bind(&Post_processing_SISO<B,Q>::siso_i, pp, _1, _2, _3    ));
			decoder->add_handler_end   (std::bind(&Post_processing_SISO<B,Q>::end,    pp, _1            ));
		}
	}

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_turbo<B_8,Q_8,QD_8>;
template class aff3ct::tools::Codec_turbo<B_16,Q_16,QD_16>;
template class aff3ct::tools::Codec_turbo<B_32,Q_32,QD_32>;
template class aff3ct::tools::Codec_turbo<B_64,Q_64,QD_64>;
#else
template class aff3ct::tools::Codec_turbo<B,Q,QD>;
#endif
// ==================================================================================== explicit template instantiation
