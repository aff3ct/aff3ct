#include <stdexcept>

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
		fb_generator = Factory_frozenbits_generator<B>::build(params);

		if (fb_generator == nullptr)
			throw std::runtime_error("aff3ct::tools::Codec_polar: \"fb_generator\" can't be null.");
	}
	else
	{
		if (this->params.code.N != this->params.code.N_code)
			throw std::invalid_argument("aff3ct::tools::Codec_polar: \"params.code.N\" has to be equal "
			                            "to \"params.code.N_code\".");
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
		Factory_decoder_polar_gen<B,Q>::get_frozen_bits(this->params, frozen_bits);
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
	return Factory_puncturer_polar<B,Q>::build(this->params, fb_generator);
}

template <typename B, typename Q>
Encoder<B>* Codec_polar<B,Q>
::build_encoder(const int tid, const module::Interleaver<int>* itl)
{
	return Factory_encoder_polar<B>::build(this->params, frozen_bits);
}

template <typename B, typename Q>
SISO<Q>* Codec_polar<B,Q>
::build_siso(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
{
	decoder_siso[tid] = Factory_decoder_polar<B,Q>::build_siso(this->params, frozen_bits);
	return decoder_siso[tid];
}

template <typename B, typename Q>
Decoder<B,Q>* Codec_polar<B,Q>
::build_decoder(const int tid, const module::Interleaver<int>* itl, module::CRC<B>* crc)
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
			return Factory_decoder_polar_gen<B,Q>::build(this->params, frozen_bits, crc);
		else
			return Factory_decoder_polar<B,Q>::build(this->params, frozen_bits, crc);
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
