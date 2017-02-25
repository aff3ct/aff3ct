#ifndef SIMULATION_LDPC_HPP_
#define SIMULATION_LDPC_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class Simulation_BFER_LDPC : public Simulation_BFER<B,R,Q>
{
protected:
	tools::AList_reader alist_data;

public:
	Simulation_BFER_LDPC(const tools::parameters& params);

	virtual ~Simulation_BFER_LDPC();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_LDPC_HPP_ */
