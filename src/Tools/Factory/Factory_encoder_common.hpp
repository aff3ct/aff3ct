#ifndef FACTORY_ENCODER_COMMON_HPP
#define FACTORY_ENCODER_COMMON_HPP

#include "Module/Encoder/Encoder.hpp"

#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B>
struct Factory_encoder_common : public Factory
{
	static module::Encoder<B>* build(const parameters &params, const int seed = 0, const int n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_COMMON_HPP */
