#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include <string>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Encoder/Encoder.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_polar : public Factory
{
	static module::Encoder<B>* build(const std::string      type,
	                                 const int              K,
	                                 const int              N,
	                                 const mipp::vector<B> &frozen_bits,
	                                 const bool             sys_encoding = true,
	                                 const int              n_frames     = 1);
};
}
}

#endif /* FACTORY_ENCODER_POLAR_HPP */
