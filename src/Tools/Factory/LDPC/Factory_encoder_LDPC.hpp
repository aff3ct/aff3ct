#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include "../../../Encoder/LDPC/Encoder_LDPC_sys.hpp"

#include "../../params.h"

template <typename B>
struct Factory_encoder_LDPC
{
	static Encoder_LDPC_sys<B>* build(const t_simulation_param &simu_params,
	                                  const t_code_param       &code_params,
	                                  const t_encoder_param    &enco_params,
	                                  const int                 coset_seed = 0);
};

#endif /* FACTORY_ENCODER_LDPC_HPP */
