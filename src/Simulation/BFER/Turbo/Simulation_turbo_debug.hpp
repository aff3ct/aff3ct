#ifndef SIMULATION_TURBO_DEBUG_HPP_
#define SIMULATION_TURBO_DEBUG_HPP_

#include <vector>

#include "Simulation_turbo.hpp"

#include "../../../Tools/Frame_trace/Frame_trace.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo_debug : public Simulation_turbo<B,R,Q,QD>
{
private:
	Frame_trace<B> ft;

public:
	Simulation_turbo_debug(const t_simulation_param& simu_params,
	                       const t_code_param&       code_params,
	                       const t_encoder_param&    enco_params,
	                       const t_channel_param&    chan_params,
	                       const t_decoder_param&    deco_params);

	virtual ~Simulation_turbo_debug();

protected:
	void simulation_loop();
};

#endif /* SIMULATION_TURBO_DEBUG_HPP_ */
