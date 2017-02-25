#ifndef SIMULATION_RSC_HPP_
#define SIMULATION_RSC_HPP_

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q, typename QD>
class Simulation_BFER_RSC : public Simulation_BFER<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Simulation_BFER_RSC(const tools::parameters& params);
	virtual ~Simulation_BFER_RSC();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_RSC_HPP_ */
