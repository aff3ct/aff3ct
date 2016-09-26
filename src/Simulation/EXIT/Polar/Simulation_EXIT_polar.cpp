#include <cassert>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iostream>

#include "Tools/bash_tools.h"
#include "Tools/simu_tools.h"

#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Factory/Polar/Factory_encoder_polar.hpp"
#include "Tools/Factory/Polar/Factory_decoder_polar.hpp"

#include "Simulation_EXIT_polar.hpp"

template <typename B, typename R, typename Q>
Simulation_EXIT_polar<B,R,Q>
::Simulation_EXIT_polar(const t_simulation_param& simu_params,
                        const t_code_param&       code_params,
                        const t_encoder_param&    enco_params,
                        const t_mod_param&        mod_params,
                        const t_channel_param&    chan_params,
                        const t_decoder_param&    deco_params)
: Simulation_EXIT<B,R,Q>(simu_params, code_params, enco_params, mod_params, chan_params, deco_params),
  frozen_bits(code_params.N),
  fb_generator(nullptr)
{
	// fixed fronzen bits is mandatory
	assert(code_params.sigma != 0.f);

	// build the frozen bits generator
	fb_generator = Factory_frozenbits_generator<B>::build(simu_params, code_params);
	check_errors(fb_generator, "Frozenbits_generator<B>");
}

template <typename B, typename R, typename Q>
Simulation_EXIT_polar<B,R,Q>
::~Simulation_EXIT_polar()
{
	delete fb_generator;
}

template <typename B, typename R, typename Q>
void Simulation_EXIT_polar<B,R,Q>
::extract_sys_par(const mipp::vector<Q> &Lch_N, 
                  const mipp::vector<Q> &La_K, 
                        mipp::vector<Q> &sys, 
                        mipp::vector<Q> &par)
{
	// extract systematic and parity information
	auto par_idx = 0, sys_idx = 0;
	for (auto j = 0; j < this->code_params.N; j++)
		if (!frozen_bits[j]) // add La on information (systematic) bits
		{
			sys[sys_idx] = Lch_N[j] + La_K[sys_idx];
			sys_idx++;
		}
		else // parity bit
			par[par_idx++] = Lch_N[j];
}

template <typename B, typename R, typename Q>
void Simulation_EXIT_polar<B,R,Q>
::launch_precompute()
{
	// static generation of the frozen bits in order to get a single polar code for the whole simulation
	fb_generator->generate(frozen_bits);
}

template <typename B, typename R, typename Q>
void Simulation_EXIT_polar<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_EXIT_polar<B,R,Q>
::build_encoder()
{
	return Factory_encoder_polar<B>::build(this->code_params, this->enco_params, this->deco_params, frozen_bits);
}

template <typename B, typename R, typename Q>
SISO<Q>* Simulation_EXIT_polar<B,R,Q>
::build_siso()
{
	return Factory_decoder_polar<B,Q>::build_siso(this->code_params,
	                                              this->enco_params,
	                                              this->chan_params,
	                                              this->deco_params,
	                                              frozen_bits);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_EXIT_polar<B_8,R_8,Q_8>;
template class Simulation_EXIT_polar<B_16,R_16,Q_16>;
template class Simulation_EXIT_polar<B_32,R_32,Q_32>;
template class Simulation_EXIT_polar<B_64,R_64,Q_64>;
#else
template class Simulation_EXIT_polar<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
