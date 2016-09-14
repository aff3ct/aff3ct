#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "Tools/params.h"

template <typename B>
struct Factory_encoder_RSC
{
	static Encoder_RSC_sys<B>* build(const parameters &params, const int n_frames = 1);
};

#endif /* FACTORY_ENCODER_RSC_HPP */
