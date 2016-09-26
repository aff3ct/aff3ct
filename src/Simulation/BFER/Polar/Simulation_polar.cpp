#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/bash_tools.h"
#include "Tools/simu_tools.h"
#include "Tools/Frame_trace/Frame_trace.hpp"

#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Factory/Polar/Factory_puncturer_polar.hpp"
#include "Tools/Factory/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar_gen.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "Simulation_polar.hpp"

template <typename B, typename R, typename Q>
Simulation_polar<B,R,Q>
::Simulation_polar(const t_simulation_param& simu_params,
                   const t_code_param&       code_params,
                   const t_encoder_param&    enco_params,
                   const t_mod_param&        mod_params,
                   const t_channel_param&    chan_params,
                   const t_decoder_param&    deco_params)
: Simulation_BFER<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params),
  frozen_bits(std::exp2(this->code_params.m)),
  is_generated_decoder((deco_params.implem.find("_SNR") != std::string::npos) && (deco_params.algo == "SC")),
  fb_generator(nullptr)
{
	const_cast<t_code_param&>(this->code_params).N_code = std::exp2(this->code_params.m);

	if (!is_generated_decoder)
	{
		// build the frozen bits generator
		fb_generator = Factory_frozenbits_generator<B>::build(simu_params, code_params);
		check_errors(fb_generator, "Frozenbits_generator<B>");
	}
	else
		assert(this->code_params.N == this->code_params.N_code);


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
		if (!this->simu_params.awgn_codes_file.empty() || this->code_params.sigma != 0.f)
		{
			fb_generator->generate(frozen_bits);
			if (this->code_params.N != this->code_params.N_code)
			{
				Puncturer_polar_wangliu<B,Q> punct(this->code_params.N, this->code_params.K, *fb_generator);
				punct.gen_frozen_bits(frozen_bits);
			}
		}
	}
	else
		Factory_decoder_polar_gen<B,Q>::get_frozen_bits(this->code_params, this->deco_params, frozen_bits);
}

template <typename B, typename R, typename Q>
void Simulation_polar<B,R,Q>
::snr_precompute()
{
	// adaptative frozen bits generation
	if (this->simu_params.awgn_codes_file.empty() && this->code_params.sigma == 0.f && !is_generated_decoder)
	{
		fb_generator->set_sigma(this->sigma);		
		fb_generator->generate(frozen_bits);

		if (this->code_params.N != this->code_params.N_code)
		{
			Puncturer_polar_wangliu<B,Q> punct(this->code_params.N, this->code_params.K, *fb_generator);
			punct.gen_frozen_bits(frozen_bits);
		}
	}

	if (this->simu_params.enable_debug)
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
	return Factory_puncturer_polar<B,Q>::build(this->code_params, this->deco_params, fb_generator);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_polar<B,R,Q>
::build_encoder(const int tid)
{
	return Factory_encoder_polar<B>::build(this->code_params, this->enco_params, this->deco_params, frozen_bits);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_polar<B,R,Q>
::build_decoder(const int tid)
{
	if (is_generated_decoder)
		return Factory_decoder_polar_gen<B,Q>::build(this->code_params,
		                                             this->enco_params,
		                                             this->chan_params,
		                                             this->deco_params,
		                                             frozen_bits);
	else
		return Factory_decoder_polar<B,Q>::build(this->code_params,
		                                         this->enco_params,
		                                         this->chan_params,
		                                         this->deco_params,
		                                         frozen_bits,
		                                         this->crc[tid]);
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
