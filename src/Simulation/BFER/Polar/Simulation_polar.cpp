#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Factory/Polar/Factory_puncturer_polar.hpp"
#include "Tools/Factory/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar_gen.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Simulation_polar.hpp"

template <typename B, typename R, typename Q>
Simulation_polar<B,R,Q>
::Simulation_polar(const parameters& params)
: Simulation_BFER<B,R,Q>(params),
  frozen_bits(std::exp2(this->params.code.m)),
  is_generated_decoder((params.decoder.implem.find("_SNR") != std::string::npos) && (params.decoder.algo == "SC")),
  fb_generator(nullptr)
{
	const_cast<code_parameters&>(this->params.code).N_code = std::exp2(this->params.code.m);

	if (!is_generated_decoder)
	{
		// build the frozen bits generator
		fb_generator = Factory_frozenbits_generator<B>::build(params);
		Simulation::check_errors(fb_generator, "Frozenbits_generator<B>");
	}
	else
		assert(this->params.code.N == this->params.code.N_code);


}

template <typename B, typename R, typename Q>
Simulation_polar<B,R,Q>
::~Simulation_polar()
{
	if (fb_generator != nullptr) { delete fb_generator; fb_generator = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::launch_precompute()
{
	if (!is_generated_decoder)
	{
		if (!this->params.simulation.awgn_codes_file.empty() || this->params.code.sigma != 0.f)
		{
			fb_generator->generate(frozen_bits);
			if (this->params.code.N != this->params.code.N_code)
			{
				Puncturer_polar_wangliu<B,Q> punct(this->params.code.N, this->params.code.K, *fb_generator);
				punct.gen_frozen_bits(frozen_bits);
			}
		}
	}
	else
		Factory_decoder_polar_gen<B,Q>::get_frozen_bits(this->params, frozen_bits);
}

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::snr_precompute()
{
	// adaptative frozen bits generation
	if (this->params.simulation.awgn_codes_file.empty() && this->params.code.sigma == 0.f && !is_generated_decoder)
	{
		fb_generator->set_sigma(this->sigma);
		fb_generator->generate(frozen_bits);

		if (this->params.code.N != this->params.code.N_code)
		{
			Puncturer_polar_wangliu<B,Q> punct(this->params.code.N, this->params.code.K, *fb_generator);
			punct.gen_frozen_bits(frozen_bits);
		}
	}

	if (this->params.simulation.enable_debug)
	{
		std::clog << std::endl << "Frozen bits:" << std::endl;
		Frame_trace<B> ft;
		ft.display_bit_vector(frozen_bits);
		std::clog << std::endl;
	}
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_polar<B,R,Q>
::build_puncturer(const int tid)
{
	return Factory_puncturer_polar<B,Q>::build(this->params, fb_generator);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_polar<B,R,Q>
::build_encoder(const int tid)
{
	return Factory_encoder_polar<B>::build(this->params, frozen_bits);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_polar<B,R,Q>
::build_decoder(const int tid)
{
	if (is_generated_decoder)
		return Factory_decoder_polar_gen<B,Q>::build(this->params, frozen_bits);
	else
		return Factory_decoder_polar<B,Q>::build(this->params, frozen_bits, this->crc[tid]);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_polar<B_8,R_8,Q_8>;
template class Simulation_polar<B_16,R_16,Q_16>;
template class Simulation_polar<B_32,R_32,Q_32>;
template class Simulation_polar<B_64,R_64,Q_64>;
#else
template class Simulation_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
