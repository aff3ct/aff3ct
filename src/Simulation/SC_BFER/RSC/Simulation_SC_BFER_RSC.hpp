#ifndef SIMULATION_SC_BFER_RSC_HPP_
#define SIMULATION_SC_BFER_RSC_HPP_

#include "../Simulation_SC_BFER.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_SC_BFER_RSC : public Simulation_SC_BFER<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Simulation_SC_BFER_RSC(const t_simulation_param& simu_params,
	                       const t_code_param&       code_params,
	                       const t_encoder_param&    enco_params,
	                       const t_mod_param&        mod_params,
	                       const t_channel_param&    chan_params,
	                       const t_decoder_param&    deco_params);
	virtual ~Simulation_SC_BFER_RSC();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    ();
	Decoder<B,Q>* build_decoder    ();
};

#endif /* SIMULATION_SC_BFER_RSC_HPP_ */
