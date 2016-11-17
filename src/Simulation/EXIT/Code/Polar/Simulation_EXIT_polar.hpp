#ifndef SIMULATION_EXIT_POLAR_HPP_
#define SIMULATION_EXIT_POLAR_HPP_

#include <chrono>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Simulation/EXIT/Simulation_EXIT.hpp"

template <typename B, typename R, typename Q>
class Simulation_EXIT_polar : public Simulation_EXIT<B,R,Q>
{
protected:
	// frozen bits vectors
	mipp::vector<B>          frozen_bits; // known bits (alias frozen bits) are set to true
	Frozenbits_generator<B> *fb_generator;
	
public:
	Simulation_EXIT_polar(const parameters& params);
	virtual ~Simulation_EXIT_polar();

protected:
	void extract_sys_par(const mipp::vector<R> &Lch_N, 
	                     const mipp::vector<R> &La_K, 
	                           mipp::vector<R> &sys, 
	                           mipp::vector<R> &par);

	void        launch_precompute();
	void        snr_precompute   ();
	Encoder<B>* build_encoder    ();
	SISO<R>*    build_siso       ();
};

#endif /* SIMULATION_EXIT_POLAR_HPP_ */
