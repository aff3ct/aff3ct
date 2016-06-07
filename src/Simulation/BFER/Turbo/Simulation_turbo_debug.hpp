#ifndef SIMULATION_TURBO_DEBUG_HPP_
#define SIMULATION_TURBO_DEBUG_HPP_

#include <vector>

#include "Simulation_turbo.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo_debug : public Simulation_turbo<B,R,Q,QD>
{
public:
	Simulation_turbo_debug(const t_simulation_param& simu_params,
	                       const t_code_param&       code_params,
	                       const t_encoder_param&    enco_params,
	                       const t_channel_param&    chan_params,
	                       const t_decoder_param&    deco_params);

	virtual ~Simulation_turbo_debug();

protected:
	void simulation_loop();

private:
	void display_bit_vector(mipp::vector<B> vec);
	void display_real_vector(mipp::vector<R> vec);
	void display_quantized_vector(mipp::vector<Q> vec);
};

#endif /* SIMULATION_TURBO_DEBUG_HPP_ */
