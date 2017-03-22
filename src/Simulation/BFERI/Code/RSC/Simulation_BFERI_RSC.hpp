#ifndef SIMULATION_BFERI_RSC_HPP_
#define SIMULATION_BFERI_RSC_HPP_

#include "Simulation/BFERI/Standard/STD_Simulation_BFERI.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Simulation_BFERI_RSC : public Simulation_BFERI<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Simulation_BFERI_RSC(const tools::parameters& params);
	virtual ~Simulation_BFERI_RSC();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::SISO<Q>*      build_siso       (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_BFERI_RSC_HPP_ */
