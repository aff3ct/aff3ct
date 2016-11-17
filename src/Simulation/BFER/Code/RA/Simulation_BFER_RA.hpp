#ifndef SIMULATION_RA_HPP_
#define SIMULATION_RA_HPP_

#include "Module/Interleaver/Interleaver.hpp"

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFER_RA : public Simulation_BFER<B,R,Q>
{
protected:
	Interleaver<int> *interleaver;

public:
	Simulation_BFER_RA(const parameters& params);
	virtual ~Simulation_BFER_RA();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_RA_HPP_ */
