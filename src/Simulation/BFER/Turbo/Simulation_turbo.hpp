#ifndef SIMULATION_TURBO_HPP_
#define SIMULATION_TURBO_HPP_

#include "../Simulation_BFER.hpp"

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Encoder/RSC/Encoder_RSC_sys.hpp"
#include "../../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"
#include "../../../Decoder/SISO.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q, typename QD>
class Simulation_turbo : public Simulation_BFER<B,R,Q>
{
protected:
	// the trellis representation
	mipp::vector<mipp::vector<int>>  trellis;
	Interleaver<short>              *interleaver;
	std::vector<Encoder_RSC_sys<B>*> sub_encoder;
	std::vector<SISO<Q>*>            siso;
	std::vector<Scaling_factor<Q>*>  sf;

public:
	Simulation_turbo(const t_simulation_param& simu_params,
	                 const t_code_param&       code_params,
	                 const t_encoder_param&    enco_params,
	                 const t_channel_param&    chan_params,
	                 const t_decoder_param&    deco_params);
	virtual ~Simulation_turbo();

protected:
	void          release_objects  ();
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_TURBO_HPP_ */
