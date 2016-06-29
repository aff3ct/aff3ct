#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include "../../../Interleaver/Interleaver.hpp"
#include "../../../Encoder/Encoder.hpp"
#include "../../../Encoder/Encoder_sys.hpp"

#include "../../params.h"

template <typename B>
struct Factory_encoder_turbo
{
	static Encoder<B>* build(const t_code_param        &code_params,
	                         const t_encoder_param     &enco_params,
	                         const Interleaver<short>  *interleaver,
	                               Encoder_sys<B>      *sub_encoder_n,
	                               Encoder_sys<B>      *sub_encoder_i,
	                         const int                 n_frames = 1);
};

#endif /* FACTORY_ENCODER_TURBO_HPP */