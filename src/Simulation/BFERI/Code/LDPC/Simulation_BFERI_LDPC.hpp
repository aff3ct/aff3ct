#ifndef SIMULATION_BFERI_LDPC_HPP_
#define SIMULATION_BFERI_LDPC_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Simulation/BFERI/Standard/STD_Simulation_BFERI.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class Simulation_BFERI_LDPC : public Simulation_BFERI<B,R,Q>
{
protected:
	tools::AList_reader alist_data;

	std::vector<module::Decoder_SISO<B,Q>*> decoder_siso;

public:
	Simulation_BFERI_LDPC(const tools::parameters& params);

	virtual ~Simulation_BFERI_LDPC();

protected:
	void                  launch_precompute();
	void                  snr_precompute   ();
	module::Encoder<B>*   build_encoder    (const int tid = 0);
	module::SISO<Q>*      build_siso       (const int tid = 0);
	module::Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}
}

#endif /* SIMULATION_BFERI_LDPC_HPP_ */
