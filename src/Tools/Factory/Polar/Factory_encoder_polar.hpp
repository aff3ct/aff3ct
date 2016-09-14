#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include "Tools/params.h"
#include "Tools/MIPP/mipp.h"

#include "Module/Encoder/Encoder.hpp"

template <typename B>
struct Factory_encoder_polar
{
	static Encoder<B>* build(const parameters &params, const mipp::vector<B> &frozen_bits, const int n_frames = 1);
};

#endif /* FACTORY_ENCODER_POLAR_HPP */
