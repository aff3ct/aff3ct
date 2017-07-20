#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Factory/Module/Code/Polar/Decoder_polar_gen.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Codec_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_polar<B,Q>
::Codec_polar(const factory::Frozenbits_generator::parameters &fb_params,
              const factory::Puncturer_polar     ::parameters &pct_params,
              const factory::Encoder_polar       ::parameters &enc_params,
              const factory::Decoder_polar       ::parameters &dec_params,
              const int n_threads)
: Codec_SISO<B,Q>(enc_params, dec_params),
  fb_par(fb_params),
  pct_par(pct_params),
  enc_par(enc_params),
  dec_par(dec_params),
  frozen_bits(fb_par.N_cw),
  is_generated_decoder((dec_params.implem.find("_SNR") != std::string::npos)),
  fb_generator(nullptr),
  decoder_siso(n_threads, nullptr)
{
	if (!is_generated_decoder)
	{
		// build the frozen bits generator
		fb_generator = factory::Frozenbits_generator::build<B>(fb_par);

		if (fb_generator == nullptr)
			throw runtime_error(__FILE__, __LINE__, __func__, "'fb_generator' can't be null.");
	}
	else
	{
		if (dec_par.N_cw != pct_par.N)
		{
			std::stringstream message;
			message << "'N_cw' has to be equal to 'N' ('N_cw' = " << dec_par.N_cw
			        << ", 'N' = " << pct_par.N << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}
}

template <typename B, typename Q>
Codec_polar<B,Q>
::~Codec_polar()
{
	if (fb_generator != nullptr) { delete fb_generator; fb_generator = nullptr; }
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::launch_precompute()
{
	if (!is_generated_decoder)
	{
		if (fb_par.sigma != -1.f)
		{
			fb_generator->generate(frozen_bits);
			if (dec_par.N_cw != pct_par.N)
			{
				Puncturer_polar_wangliu<B,Q> punct(pct_par.K, pct_par.N, *fb_generator);
				punct.gen_frozen_bits(frozen_bits);
			}
		}
	}
	else
		factory::Decoder_polar_gen::get_frozen_bits<B>(dec_par.implem, dec_par.N_cw, frozen_bits);
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::snr_precompute(const float sigma)
{
	// adaptative frozen bits generation
	if (fb_par.sigma == -1.f && !is_generated_decoder)
	{
		fb_generator->set_sigma(sigma);
		fb_generator->generate(frozen_bits);

		if (dec_par.N_cw != pct_par.N)
		{
			Puncturer_polar_wangliu<B,Q> punct(pct_par.K, pct_par.N, *fb_generator);
			punct.gen_frozen_bits(frozen_bits);
		}
	}
}

template <typename B, typename Q>
Puncturer<B,Q>* Codec_polar<B,Q>
::build_puncturer(const int tid)
{
	if(fb_generator == nullptr)
		throw runtime_error(__FILE__, __LINE__, __func__, "Polar puncturer requires a frozen bits generator.");

	return factory::Puncturer_polar::build<B,Q>(pct_par, *fb_generator);
}

template <typename B, typename Q>
Encoder<B>* Codec_polar<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return factory::Encoder_polar::build<B>(this->enc_params, frozen_bits);
}

template <typename B, typename Q>
SISO<Q>* Codec_polar<B,Q>
::build_siso(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	decoder_siso[tid] = factory::Decoder_polar::build_siso<B,Q>(dec_par, frozen_bits);
	return decoder_siso[tid];
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_polar<B,Q>
::build_decoder(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	if (decoder_siso[tid] != nullptr)
	{
		auto ptr = decoder_siso[tid];
		decoder_siso[tid] = nullptr;
		return ptr;
	}
	else
	{
		if (is_generated_decoder)
			return factory::Decoder_polar_gen::build<B,Q>(dec_par, frozen_bits, crc);
		else
			return factory::Decoder_polar    ::build<B,Q>(dec_par, frozen_bits, crc);
	}
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K = dec_par.K;
	const auto N = dec_par.N_cw;

	if ((int)Y_N.size() != N * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'inter_frame_level' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << N << ", 'inter_frame_level' = " << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)sys.size() != K * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to 'K' * 'inter_frame_level' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << K << ", 'inter_frame_level' = " << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)par.size() != (N - K) * dec_par.n_frames)
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to ('N' - 'K') * 'inter_frame_level' ('par.size()' = " << par.size()
		        << ", 'N' = " << N << ", 'K' = " << K << ", 'inter_frame_level' = "
		        << dec_par.n_frames << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	// extract systematic and parity information
	auto par_idx = 0, sys_idx = 0;
	for (auto f = 0; f < dec_par.n_frames; f++)
		for (auto i = 0; i < N; i++)
			if (!frozen_bits[i]) // add La on information (systematic) bits
			{
				sys[sys_idx] = Y_N[f * N +i];
				sys_idx++;
			}
			else // parity bit
			{
				par[par_idx] = Y_N[f * N +i];
				par_idx++;
			}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Codec_polar<B_8,Q_8>;
template class aff3ct::tools::Codec_polar<B_16,Q_16>;
template class aff3ct::tools::Codec_polar<B_32,Q_32>;
template class aff3ct::tools::Codec_polar<B_64,Q_64>;
#else
template class aff3ct::tools::Codec_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
