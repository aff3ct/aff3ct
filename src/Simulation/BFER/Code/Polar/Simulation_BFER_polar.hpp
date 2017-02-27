#ifndef SIMULATION_POLAR_HPP_
#define SIMULATION_POLAR_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER_polar : public Simulation_BFER<B,R,Q>
{
protected:
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	tools::Frozenbits_generator<B> *fb_generator;

public:
	Simulation_BFER_polar(const tools::parameters& params);
	virtual ~Simulation_BFER_polar();

protected:
	void                    launch_precompute();
	void                    snr_precompute   ();
	module::Puncturer<B,Q>* build_puncturer  (const int tid = 0);
	module::Encoder<B>*     build_encoder    (const int tid = 0);
	module::Decoder<B,Q>*   build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_POLAR_HPP_ */
