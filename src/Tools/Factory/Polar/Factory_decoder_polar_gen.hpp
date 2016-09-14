#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include "Module/Decoder/Decoder.hpp"

#include "Tools/MIPP/mipp.h"
#include "Tools/params.h"

template <typename B, typename R>
struct Factory_decoder_polar_gen
{
	static Decoder<B,R>* build(const parameters &params, const mipp::vector<B> &frozen_bits);

	static void get_frozen_bits(const parameters &params, mipp::vector<B> &frozen_bits);
};

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
