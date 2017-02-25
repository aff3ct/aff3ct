#ifndef FACTORY_DECODER_POLAR_GEN_HPP
#define FACTORY_DECODER_POLAR_GEN_HPP

#include "Module/Decoder/Decoder.hpp"
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename R>
struct Factory_decoder_polar_gen : public Factory
{
	static module::Decoder<B,R>* build(const parameters &params, const mipp::vector<B> &frozen_bits);

	static void get_frozen_bits(const parameters &params, mipp::vector<B> &frozen_bits);
};
}
}

#endif /* FACTORY_DECODER_POLAR_GEN_HPP */
