#ifndef SIMULATION_REPETITION_HPP_
#define SIMULATION_REPETITION_HPP_

#include "Tools/params.h"

#include "../Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
class Simulation_repetition : public Simulation_BFER<B,R,Q>
{
public:
	Simulation_repetition(const t_simulation_param& simu_params,
	                      const t_code_param&       code_params,
	                      const t_encoder_param&    enco_params,
	                      const t_mod_param&        mod_params,
	                      const t_channel_param&    chan_params,
	                      const t_decoder_param&    deco_params);

	virtual ~Simulation_repetition();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_REPETITION_HPP_ */
