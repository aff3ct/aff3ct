#ifndef SIMULATION_TURBO_HPP_
#define SIMULATION_TURBO_HPP_

#include "Tools/params.h"

#include "Tools/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"
#include "Module/Decoder/SISO.hpp"

#include "../Simulation_BFER.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo : public Simulation_BFER<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>>    trellis;
	Interleaver<short>              *interleaver;
	std::vector<Encoder_RSC_sys<B>*> sub_encoder;
	std::vector<SISO<Q>*>            siso;
	std::vector<Scaling_factor<Q>*>  sf;

public:
	Simulation_turbo(const parameters& params);
	virtual ~Simulation_turbo();

protected:
	void          release_objects  ();
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_TURBO_HPP_ */
