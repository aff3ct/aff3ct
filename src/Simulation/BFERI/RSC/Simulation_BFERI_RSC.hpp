#ifndef SIMULATION_BFERI_RSC_HPP_
#define SIMULATION_BFERI_RSC_HPP_

#include "../Simulation_BFERI.hpp"

template <typename B, typename R, typename Q, typename QD>
class Simulation_BFERI_RSC : public Simulation_BFERI<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;

public:
	Simulation_BFERI_RSC(const t_simulation_param& simu_params,
	                     const t_code_param&       code_params,
	                     const t_encoder_param&    enco_params,
	                     const t_mod_param&        mod_params,
	                     const t_channel_param&    chan_params,
	                     const t_decoder_param&    deco_params);
	virtual ~Simulation_BFERI_RSC();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	SISO<Q>*      build_siso       (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_BFERI_RSC_HPP_ */
