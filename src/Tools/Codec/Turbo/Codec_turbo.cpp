#include <functional>
#include <cctype>

#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"
#include "Tools/Factory/RSC/Factory_encoder_RSC.hpp"
#include "Tools/Factory/Turbo/Factory_puncturer_turbo.hpp"
#include "Tools/Factory/Turbo/Factory_scaling_factor.hpp"
#include "Tools/Factory/Turbo/Factory_flip_and_check.hpp"
#include "Tools/Factory/RSC/Factory_decoder_RSC.hpp"

#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Self_corrected/Self_corrected.hpp"

#include "Codec_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::Codec_turbo(const typename Factory_encoder_turbo  <B  >::encoder_parameters_turbo   &enc_params,
              const typename Factory_decoder_turbo  <B,Q>::decoder_parameters_turbo   &dec_params,
              const typename Factory_puncturer_turbo<B,Q>::puncturer_parameters_turbo &pct_params,
              const int n_threads)
: Codec<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params), pct_par(pct_params),
  sub_enc  (n_threads, nullptr),
  siso     (n_threads, nullptr),
  post_pros(n_threads         )
{
	if (!enc_par.json_path.empty())
	{
		if (n_threads != 1)
			throw runtime_error(__FILE__, __LINE__, __func__, "JSON trace only supports single-threaded simulation.");

		json_stream.open(enc_par.json_path.c_str(), std::ios::out | std::ios::trunc);

		json_stream << "[" << std::endl;
	}

	// fill RSC encoder/decoder parameters structures
	enc_rsc_par.type     = "RSC";
	enc_rsc_par.K        = enc_par.K;
	enc_rsc_par.N        = (enc_par.K * 2) + (enc_par.tail_length / 2);
	enc_rsc_par.buffered = enc_par.buffered;
	enc_rsc_par.poly     = enc_par.poly;
	enc_rsc_par.n_frames = enc_par.n_frames;

	auto encoder_RSC = Factory_encoder_RSC<B>::build(enc_rsc_par);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;

	enc_rsc_par.type = enc_par.json_path.empty() ? "RSC" : "RSC_JSON";

	dec_rsc_par.type          = dec_par.type,
	dec_rsc_par.max           = dec_par.max,
	dec_rsc_par.implem        = dec_par.implem,
	dec_rsc_par.simd_strategy = dec_par.simd_strategy,
	dec_rsc_par.K             = dec_par.K,
	dec_rsc_par.n_frames      = dec_par.n_frames;
}

template <typename B, typename Q, typename QD>
Codec_turbo<B,Q,QD>
::~Codec_turbo()
{
	int tid = 0;
	const int nthr = sub_enc.size();
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
	return Factory_interleaver<int>::build(enc_par.itl, seed);
}

template <typename B, typename Q, typename QD>
Encoder_RSC_sys<B>* Codec_turbo<B,Q,QD>
::build_sub_encoder(const int tid)
{
	return Factory_encoder_RSC<B>::build(enc_rsc_par, json_stream);
}

template <typename B, typename Q, typename QD>
Encoder<B>* Codec_turbo<B,Q,QD>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'itl' should not be null.");

	if (sub_enc[tid] != nullptr)
	{
		delete sub_enc[tid];
		sub_enc[tid] = nullptr;
	}

	sub_enc[tid] = this->build_sub_encoder(tid);

	if (sub_enc[tid] == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'sub_enc' can't be created.");

	return Factory_encoder_turbo<B>::build(enc_par, *itl, sub_enc[tid], sub_enc[tid]);
}

template <typename B, typename Q, typename QD>
Puncturer<B,Q>* Codec_turbo<B,Q,QD>
::build_puncturer(const int tid)
{
	return Factory_puncturer_turbo<B,Q>::build(pct_par, enc_par.tail_length, enc_par.buffered);
}

template <typename B, typename Q, typename QD>
SISO<Q>* Codec_turbo<B,Q,QD>
::build_sub_siso(const int tid)
{
	return Factory_decoder_RSC<B,Q,QD>::build(dec_rsc_par, enc_rsc_par, trellis, json_stream, dec_par.n_ite);
}

template <typename B, typename Q, typename QD>
Decoder<B,Q>* Codec_turbo<B,Q,QD>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__,  "'itl' should not be null.");

	clear_post_processing(tid);

	if (siso[tid] != nullptr)
	{
		delete siso[tid];
		siso[tid] = nullptr;
	}

	siso[tid] = this->build_sub_siso(tid);

	if (siso[tid] == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'siso' can't be created.");

	auto decoder = Factory_decoder_turbo<B,Q>::build(dec_par, *itl, *siso[tid], *siso[tid], enc_par.buffered);

// then add post processing modules
	if (!dec_par.scaling_factor.declaration.empty())
	{
		post_pros[tid].push_back(Factory_scaling_factor<B,Q>::build(dec_par.scaling_factor, dec_par.n_ite));
	}

	if (dec_par.flip_and_check.enable)
	{
		if(crc == nullptr || crc->get_size() == 0)
			throw runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

		post_pros[tid].push_back(Factory_flip_and_check<B,Q>::build(dec_par.flip_and_check,
		                                                            *crc,
		                                                            decoder->get_simd_inter_frame_level()));
	}
	else if (crc != nullptr && crc->get_size() > 0)
	{
		post_pros[tid].push_back(new tools::CRC_checker<B,Q>(*crc,
		                                                     2,
		                                                     decoder->get_simd_inter_frame_level()));
	}

	if (dec_par.self_corrected)
	{
		post_pros[tid].push_back(new tools::Self_corrected<B,Q>(dec_par.K,
		                                                        dec_par.n_ite,
		                                                        4,
		                                                        dec_par.n_ite,
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
