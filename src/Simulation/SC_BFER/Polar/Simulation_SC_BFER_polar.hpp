#ifndef SIMULATION_SC_BFER_POLAR_HPP_
#define SIMULATION_SC_BFER_POLAR_HPP_

#include "../Simulation_SC_BFER.hpp"

#include "../../../Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "../../../Tools/params.h"

template <typename B, typename R, typename Q>
class Simulation_SC_BFER_polar : public Simulation_SC_BFER<B,R,Q>
{
protected:
	mipp::vector<B> frozen_bits; // known bits (alias frozen bits) are set to true
	const bool is_generated_decoder;
	Frozenbits_generator<B> *fb_generator;

public:
	Simulation_SC_BFER_polar(const t_simulation_param& simu_params,
	                         const t_code_param&       code_params,
	                         const t_encoder_param&    enco_params,
	                         const t_mod_param&        mod_params,
	                         const t_channel_param&    chan_params,
	                         const t_decoder_param&    deco_params);
	virtual ~Simulation_SC_BFER_polar();

protected:
	void            launch_precompute();
	void            snr_precompute   ();
	Puncturer<B,Q>* build_puncturer  ();
	Encoder<B>*     build_encoder    ();
	Decoder<B,Q>*   build_decoder    ();
};

#endif /* SIMULATION_SC_BFER_POLAR_HPP_ */
