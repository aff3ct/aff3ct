#ifndef SIMULATION_POLAR_DEBUG_HPP_
#define SIMULATION_POLAR_DEBUG_HPP_

#include <vector>

#include "Simulation_polar.hpp"
#include "../../../Tools/Frame_trace/Frame_trace.hpp"

template <typename B, typename R, typename Q>
class Simulation_polar_debug : public Simulation_polar<B,R,Q>
{
private:
	Frame_trace<B> ft;

public:
	Simulation_polar_debug(const t_simulation_param& simu_params,
	                       const t_code_param&       code_params,
	                       const t_encoder_param&    enco_params,
	                       const t_channel_param&    chan_params,
	                       const t_decoder_param&    deco_params);

	virtual ~Simulation_polar_debug();

protected:
	void simulation_loop();
};

#endif /* SIMULATION_POLAR_DEBUG_HPP_ */
