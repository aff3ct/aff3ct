#ifndef SIMULATION_RSC_DEBUG_HPP_
#define SIMULATION_RSC_DEBUG_HPP_

#include <vector>

#include "Simulation_RSC.hpp"

#include "../../../Tools/Frame_trace/Frame_trace.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_RSC_debug : public Simulation_RSC<B,R,Q,QD>
{
private:
	Frame_trace<B> ft;
	
public:
	Simulation_RSC_debug(const t_simulation_param& simu_params,
	                     const t_code_param&       code_params,
	                     const t_encoder_param&    enco_params,
	                     const t_channel_param&    chan_params,
	                     const t_decoder_param&    deco_params);

	virtual ~Simulation_RSC_debug();

protected:
	void simulation_loop();
};

#endif /* SIMULATION_RSC_DEBUG_HPP_ */
