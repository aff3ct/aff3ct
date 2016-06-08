#ifndef SIMULATION_BCH_DEBUG_HPP_
#define SIMULATION_BCH_DEBUG_HPP_

#include <vector>

#include "Simulation_BCH.hpp"

template <typename B, typename R, typename Q>
class Simulation_BCH_debug : public Simulation_BCH<B,R,Q>
{
public:
	Simulation_BCH_debug(const t_simulation_param& simu_params,
	                    const t_code_param&       code_params,
	                    const t_encoder_param&    enco_params,
	                    const t_channel_param&    chan_params,
	                    const t_decoder_param&    deco_params);

	virtual ~Simulation_BCH_debug();

protected:
	void simulation_loop();

private:
	void display_bit_vector(mipp::vector<B> vec);
	void display_real_vector(mipp::vector<R> vec);
	void display_quantized_vector(mipp::vector<Q> vec);
};

#include "Simulation_BCH_debug.hxx"

#endif /* SIMULATION_BCH_DEBUG_HPP_ */
