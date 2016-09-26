#ifndef SIMULATION_EXIT_POLAR_HPP_
#define SIMULATION_EXIT_POLAR_HPP_

#include <chrono>
#include <vector>

#include "Tools/MIPP/mipp.h"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Simulation_EXIT.hpp"

template <typename B, typename R, typename Q>
class Simulation_EXIT_polar : public Simulation_EXIT<B,R,Q>
{
protected:
	// frozen bits vectors
	mipp::vector<B>          frozen_bits; // known bits (alias frozen bits) are set to true
	Frozenbits_generator<B> *fb_generator;
	
public:
	Simulation_EXIT_polar(const t_simulation_param& simu_params,
	                      const t_code_param&       code_params,
	                      const t_encoder_param&    enco_params,
	                      const t_mod_param&        mod_params,
	                      const t_channel_param&    chan_params,
	                      const t_decoder_param&    deco_params);
	virtual ~Simulation_EXIT_polar();

protected:
	void extract_sys_par(const mipp::vector<Q> &Lch_N, 
	                     const mipp::vector<Q> &La_K, 
	                           mipp::vector<Q> &sys, 
	                           mipp::vector<Q> &par);

	void        launch_precompute();
	void        snr_precompute   ();
	Encoder<B>* build_encoder    ();
	SISO<Q>*    build_siso       ();
};

#endif /* SIMULATION_EXIT_POLAR_HPP_ */
