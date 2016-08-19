#ifndef SIMULATION_SC_BFER_TURBO_HPP_
#define SIMULATION_SC_BFER_TURBO_HPP_

#include "../Simulation_SC_BFER.hpp"

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Encoder/RSC/Encoder_RSC_sys.hpp"
#include "../../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q, typename QD>
class Simulation_SC_BFER_turbo : public Simulation_SC_BFER<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>>  trellis;
	Interleaver<short>            *interleaver;
	Encoder_RSC_sys<B>            *sub_encoder;
	SISO<Q>                       *siso;
	Scaling_factor<Q>             *sf;

public:
	Simulation_SC_BFER_turbo(const t_simulation_param& simu_params,
	                         const t_code_param&       code_params,
	                         const t_encoder_param&    enco_params,
	                         const t_mod_param&        mod_params,
	                         const t_channel_param&    chan_params,
	                         const t_decoder_param&    deco_params);
	virtual ~Simulation_SC_BFER_turbo();

protected:
	void          release_objects  ();
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    ();
	Decoder<B,Q>* build_decoder    ();
};

#endif /* SIMULATION_SC_BFER_TURBO_HPP_ */
