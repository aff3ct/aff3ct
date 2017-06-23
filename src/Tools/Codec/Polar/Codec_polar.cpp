#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Factory/Polar/Factory_puncturer_polar.hpp"
#include "Tools/Factory/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar_gen.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Codec_polar.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Codec_polar<B,Q>
::Codec_polar(const parameters& params)
: Codec_SISO<B,Q>(params),
  frozen_bits(this->params.code.N_code),
  is_generated_decoder((params.decoder.implem.find("_SNR") != std::string::npos)),
  fb_generator(nullptr),
  decoder_siso(params.simulation.n_threads, nullptr)
{
	if (!is_generated_decoder)
	{
		// build the frozen bits generator
		fb_generator = Factory_frozenbits_generator<B>::build(this->params.code.fb_gen_method,
		                                                      this->params.code.K,
		                                                      this->params.code.N_code,
		                                                      this->params.code.sigma,
		                                                      this->params.code.awgn_fb_path,
		                                                      this->params.simulation.bin_pb_path);

		if (fb_generator == nullptr)
			throw runtime_error(__FILE__, __LINE__, __func__, "'fb_generator' can't be null.");
	}
	else
	{
		if (this->params.code.N != this->params.code.N_code)
		{
			std::stringstream message;
			message << "'N' has to be equal to 'N_code' ('N' = " << this->params.code.N
			        << ", 'N_code' = " << this->params.code.N_code << ").";
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
		if (this->params.code.sigma != 0.f)
		{
			fb_generator->generate(frozen_bits);
			if (this->params.code.N != this->params.code.N_code)
			{
				Puncturer_polar_wangliu<B,Q> punct(this->params.code.K, this->params.code.N, *fb_generator);
				punct.gen_frozen_bits(frozen_bits);
			}
		}
	}
	else
		Factory_decoder_polar_gen<B,Q>::get_frozen_bits(this->params.decoder.implem,
		                                                this->params.code.N_code,
		                                                frozen_bits);
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::snr_precompute(const float sigma)
{
	// adaptative frozen bits generation
	if (this->params.code.sigma == 0.f && !is_generated_decoder)
	{
		fb_generator->set_sigma(sigma);
		fb_generator->generate(frozen_bits);

		if (this->params.code.N != this->params.code.N_code)
		{
			Puncturer_polar_wangliu<B,Q> punct(this->params.code.K, this->params.code.N, *fb_generator);
			punct.gen_frozen_bits(frozen_bits);
		}
	}

	if (this->params.simulation.debug)
	{
		std::clog << std::endl << "Frozen bits:" << std::endl;
		Frame_trace<B> ft(this->params.simulation.debug_limit);
		ft.display_bit_vector(frozen_bits);
		std::clog << std::endl;
	}
}

template <typename B, typename Q>
Puncturer<B,Q>* Codec_polar<B,Q>
::build_puncturer(const int tid)
{
	const std::string type = (this->params.code.N == this->params.code.N_code) ? "NO" : "WANGLIU";
	return Factory_puncturer_polar<B,Q>::build(type,
	                                           this->params.code.K,
	                                           this->params.code.N,
	                                           fb_generator,
	                                           this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
Encoder<B>* Codec_polar<B,Q>
::build_encoder(const int tid, const Interleaver<int>* itl)
{
	return Factory_encoder_polar<B>::build(this->params.encoder.type,
	                                       this->params.code.K,
	                                       this->params.code.N_code,
	                                       frozen_bits,
	                                       this->params.encoder.systematic,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename Q>
SISO<Q>* Codec_polar<B,Q>
::build_siso(const int tid, const Interleaver<int>* itl, CRC<B>* crc)
{
	decoder_siso[tid] = Factory_decoder_polar<B,Q>::build_siso(this->params.decoder.type,
	                                                           this->params.decoder.implem,
	                                                           this->params.code.K,
	                                                           this->params.code.N_code,
	                                                           frozen_bits,
	                                                           this->params.encoder.systematic,
	                                                           this->params.decoder.n_ite,
	                                                           this->params.simulation.inter_frame_level);
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
			return Factory_decoder_polar_gen<B,Q>::build(this->params.decoder.type,
			                                             this->params.decoder.implem,
			                                             this->params.code.K,
			                                             this->params.code.N_code,
			                                             frozen_bits,
			                                             this->params.decoder.simd_strategy,
			                                             this->params.encoder.systematic,
			                                             this->params.decoder.L,
			                                             crc,
			                                             this->params.simulation.inter_frame_level);
		else
			return Factory_decoder_polar<B,Q>::build(this->params.decoder.type,
			                                         this->params.decoder.implem,
			                                         this->params.code.K,
			                                         this->params.code.N_code,
			                                         frozen_bits,
			                                         this->params.decoder.simd_strategy,
			                                         this->params.encoder.systematic,
			                                         this->params.decoder.polar_nodes,
			                                         this->params.decoder.L,
			                                         this->params.decoder.n_ite,
			                                         crc,
			                                         this->params.decoder.full_adaptive,
			                                         this->params.simulation.inter_frame_level);
	}
}

template <typename B, typename Q>
void Codec_polar<B,Q>
::extract_sys_par(const mipp::vector<Q> &Y_N, mipp::vector<Q> &sys, mipp::vector<Q> &par)
{
	const auto K = this->params.code.K;
	const auto N = this->params.code.N_code;

	if ((int)Y_N.size() != N * this->params.simulation.inter_frame_level)
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'inter_frame_level' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << N << ", 'inter_frame_level' = " << this->params.simulation.inter_frame_level << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)sys.size() != K * this->params.simulation.inter_frame_level)
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to 'K' * 'inter_frame_level' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << K << ", 'inter_frame_level' = " << this->params.simulation.inter_frame_level << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)par.size() != (N - K) * this->params.simulation.inter_frame_level)
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to ('N' - 'K') * 'inter_frame_level' ('par.size()' = " << par.size()
		        << ", 'N' = " << N << ", 'K' = " << K << ", 'inter_frame_level' = "
		        << this->params.simulation.inter_frame_level << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	// extract systematic and parity information
	auto par_idx = 0, sys_idx = 0;
	for (auto f = 0; f < this->params.simulation.inter_frame_level; f++)
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
