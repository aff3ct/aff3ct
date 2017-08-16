#include <functional>
#include <cctype>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/CRC/CRC_checker_DB.hpp"
#include "Tools/Code/Turbo/Post_processing_SISO/Flip_and_check/Flip_and_check_DB.hpp"

#include "Factory/Module/Interleaver.hpp"
#include "Factory/Tools/Code/Turbo/Scaling_factor.hpp"
#include "Factory/Tools/Code/Turbo/Flip_and_check_DB.hpp"

#include "Codec_turbo_DB.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_turbo_DB<B,Q>
::Codec_turbo_DB(const factory::Encoder_turbo_DB  ::parameters &enc_params,
                 const factory::Decoder_turbo_DB  ::parameters &dec_params,
                 const factory::Puncturer_turbo_DB::parameters &pct_params,
                 const int n_threads)
: Codec<B,Q>(enc_params, dec_params), enc_par(enc_params), dec_par(dec_params), pct_par(pct_params),
  sub_enc  (n_threads, nullptr),
  siso_n   (n_threads, nullptr),
  siso_i   (n_threads, nullptr),
  post_pros(n_threads         )
{
	auto encoder_RSC = factory::Encoder_RSC_DB::build<B>(enc_par.sub);
	trellis = encoder_RSC->get_trellis();
	delete encoder_RSC;
}

template <typename B, typename Q>
Codec_turbo_DB<B,Q>
::~Codec_turbo_DB()
{
	int tid = 0;
	const int nthr = (unsigned)sub_enc.size();
	for (tid = 0; tid < nthr; tid++) if (sub_enc[tid] != nullptr) { delete sub_enc[tid]; sub_enc[tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso_n [tid] != nullptr) { delete siso_n [tid]; siso_n [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) if (siso_i [tid] != nullptr) { delete siso_i [tid]; siso_i [tid] = nullptr; }
	for (tid = 0; tid < nthr; tid++) clear_post_processing(tid);
}

template <typename B, typename Q>
void Codec_turbo_DB<B,Q>
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

template <typename B, typename Q>
tools::Interleaver_core<>* Codec_turbo_DB<B,Q>
::build_interleaver(const int tid, const int seed)
{
	auto itl_cpy = enc_par.itl;
	itl_cpy.core.seed = enc_par.itl.core.uniform ? seed : enc_par.itl.core.seed;
	return factory::Interleaver_core::build(itl_cpy.core);
}

template <typename B, typename Q>
module::Encoder_RSC_DB<B>* Codec_turbo_DB<B,Q>
::build_sub_encoder(const int tid)
{
	return factory::Encoder_RSC_DB::build<B>(enc_par.sub);
}

template <typename B, typename Q>
module::Encoder<B>* Codec_turbo_DB<B,Q>
::build_encoder(const int tid, const module::Interleaver<B>* itl)
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

	return factory::Encoder_turbo_DB::build<B>(enc_par, *itl, *sub_enc[tid]);
}

template <typename B, typename Q>
module::Puncturer<B,Q>* Codec_turbo_DB<B,Q>
::build_puncturer(const int tid)
{
	return factory::Puncturer_turbo_DB::build<B,Q>(pct_par);
}

template <typename B, typename Q>
module::Decoder_RSC_DB_BCJR<B,Q>* Codec_turbo_DB<B,Q>
::build_sub_siso(const int tid)
{
	return factory::Decoder_RSC_DB::build<B,Q>(dec_par.sub, trellis);
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>* Codec_turbo_DB<B,Q>
::build_decoder(const int tid, const module::Interleaver<Q>* itl, module::CRC<B>* crc)
{
	if (itl == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__,  "'itl' should not be null.");

	clear_post_processing(tid);

	if (siso_n[tid] != nullptr) { delete siso_n[tid]; siso_n[tid] = nullptr; }
	if (siso_i[tid] != nullptr) { delete siso_i[tid]; siso_i[tid] = nullptr; }

	siso_n[tid] = this->build_sub_siso(tid);
	if (siso_n[tid] == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'siso_n' can't be created.");

	siso_i[tid] = this->build_sub_siso(tid);
	if (siso_i[tid] == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "'siso_i' can't be created.");

	auto decoder = factory::Decoder_turbo_DB::build<B,Q>(dec_par, *itl, *siso_n[tid], *siso_i[tid]);

	// then add post processing modules
	if (dec_par.sf.enable)
	{
		post_pros[tid].push_back(factory::Scaling_factor::build<B,Q>(dec_par.sf));
	}

	if (dec_par.fnc.enable)
	{
		if(crc == nullptr || crc->get_size() == 0)
			throw runtime_error(__FILE__, __LINE__, __func__, "The Flip aNd Check requires a CRC.");

		post_pros[tid].push_back(factory::Flip_and_check_DB::build<B,Q>(dec_par.fnc, *crc));
	}
	else if (crc != nullptr && crc->get_size() > 0)
	{
		post_pros[tid].push_back(new tools::CRC_checker_DB<B,Q>(*crc,
		                                                        2,
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
template class aff3ct::tools::Codec_turbo_DB<B_8,Q_8>;
template class aff3ct::tools::Codec_turbo_DB<B_16,Q_16>;
template class aff3ct::tools::Codec_turbo_DB<B_32,Q_32>;
template class aff3ct::tools::Codec_turbo_DB<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_turbo_DB<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
