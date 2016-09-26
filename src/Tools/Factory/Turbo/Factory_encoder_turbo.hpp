#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include "Tools/params.h"

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

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
