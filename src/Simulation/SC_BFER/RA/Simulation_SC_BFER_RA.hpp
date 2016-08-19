#ifndef SIMULATION_SC_BFER_RA_HPP_
#define SIMULATION_SC_BFER_RA_HPP_

#include "../Simulation_SC_BFER.hpp"

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q>
class Simulation_SC_BFER_RA : public Simulation_SC_BFER<B,R,Q>
{
protected:
	Interleaver<int> *interleaver;

public:
	Simulation_SC_BFER_RA(const t_simulation_param& simu_params,
	                      const t_code_param&       code_params,
	                      const t_encoder_param&    enco_params,
	                      const t_mod_param&        mod_params,
	                      const t_channel_param&    chan_params,
	                      const t_decoder_param&    deco_params);
	virtual ~Simulation_SC_BFER_RA();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    ();
	Decoder<B,Q>* build_decoder    ();
};

#endif /* SIMULATION_SC_BFER_RA_HPP_ */
