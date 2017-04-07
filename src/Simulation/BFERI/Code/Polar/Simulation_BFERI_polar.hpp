#ifndef SIMULATION_BFERI_POLAR_HPP_
#define SIMULATION_BFERI_POLAR_HPP_

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Simulation/BFERI/Standard/STD_Simulation_BFERI.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFERI_polar : public Simulation_BFERI<B,R,Q>
{
protected:
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	tools::Frozenbits_generator<B> *fb_generator;
	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Simulation_BFERI_polar(const tools::parameters& params);

	virtual ~Simulation_BFERI_polar();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::SISO<Q>*      build_siso       (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_BFERI_POLAR_HPP_ */
