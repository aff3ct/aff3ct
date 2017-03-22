#ifndef FACTORY_ENCODER_BCH_HPP
#define FACTORY_ENCODER_BCH_HPP

#include "Module/Encoder/Encoder.hpp"
#include "Tools/params.h"
#include "Tools/Math/Algebraic/Galois.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_BCH : public Factory
{
	static module::Encoder<B>* build(const parameters &params, const Galois &GF);
};
}
}

#endif /* FACTORY_ENCODER_BCH_HPP */
