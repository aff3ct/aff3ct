#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B>
struct Factory_encoder_turbo : public Factory
{
	static Encoder<B>* build(const parameters          &params,
	                         const Interleaver<short>  *interleaver,
	                               Encoder_sys<B>      *sub_encoder_n,
	                               Encoder_sys<B>      *sub_encoder_i,
	                         const int                 n_frames = 1);
};

#endif /* FACTORY_ENCODER_TURBO_HPP */
