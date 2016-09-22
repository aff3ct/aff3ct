#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include "Module/Encoder/Encoder.hpp"
#include "Tools/params.h"
#include "Tools/Perf/MIPP/mipp.h"

#include "../Factory.hpp"

template <typename B>
struct Factory_encoder_polar : public Factory
{
	static Encoder<B>* build(const parameters &params, const mipp::vector<B> &frozen_bits, const int n_frames = 1);
};

#endif /* FACTORY_ENCODER_POLAR_HPP */
