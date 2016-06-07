#ifndef SIMULATION_RSC_DEBUG_HPP_
#define SIMULATION_RSC_DEBUG_HPP_

#include <vector>

#include "Simulation_RSC.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_RSC_debug : public Simulation_RSC<B,R,Q,QD>
{
public:
	Simulation_RSC_debug(const t_simulation_param& simu_params,
	                     const t_code_param&       code_params,
	                     const t_encoder_param&    enco_params,
	                     const t_channel_param&    chan_params,
	                     const t_decoder_param&    deco_params);

	virtual ~Simulation_RSC_debug();

protected:
	void simulation_loop();

private:
	void display_bit_vector(mipp::vector<B> vec);
	void display_real_vector(mipp::vector<R> vec);
	void display_quantized_vector(mipp::vector<Q> vec);
};

#endif /* SIMULATION_RSC_DEBUG_HPP_ */
