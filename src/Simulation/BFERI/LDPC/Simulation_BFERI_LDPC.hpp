#ifndef SIMULATION_BFERI_LDPC_HPP_
#define SIMULATION_BFERI_LDPC_HPP_

#include "Tools/params.h"

#include "Module/Decoder/Decoder_SISO.hpp"

#include "../Simulation_BFERI.hpp"

template <typename B, typename R, typename Q>
class Simulation_BFERI_LDPC : public Simulation_BFERI<B,R,Q>
{
protected:
	// LDPC bi-partite graph representation
	std::vector<unsigned char> n_variables_per_parity;
	std::vector<unsigned char> n_parities_per_variable;
	std::vector<unsigned int > transpose;

	std::vector<Decoder_SISO<B,Q>*> decoder_siso;

public:
	Simulation_BFERI_LDPC(const t_simulation_param& simu_params,
	                      const t_code_param&       code_params,
	                      const t_encoder_param&    enco_params,
	                      const t_mod_param&        mod_params,
	                      const t_channel_param&    chan_params,
	                      const t_decoder_param&    deco_params);

	virtual ~Simulation_BFERI_LDPC();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	SISO<Q>*      build_siso       (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};

#endif /* SIMULATION_BFERI_LDPC_HPP_ */
