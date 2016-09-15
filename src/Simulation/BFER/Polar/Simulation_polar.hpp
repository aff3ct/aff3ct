#ifndef SIMULATION_POLAR_HPP_
#define SIMULATION_POLAR_HPP_

#include "Tools/params.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
class Simulation_polar : public Simulation_BFER<B,R,Q>
{
protected:
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

public:
	Simulation_polar(const parameters& params);
	virtual ~Simulation_polar();

protected:
	void            launch_precompute();
	void            snr_precompute   ();
	Puncturer<B,Q>* build_puncturer  (const int tid = 0);
	Encoder<B>*     build_encoder    (const int tid = 0);
	Decoder<B,Q>*   build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_POLAR_HPP_ */
