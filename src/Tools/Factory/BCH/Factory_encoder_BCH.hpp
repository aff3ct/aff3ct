#ifndef FACTORY_ENCODER_BCH_HPP
#define FACTORY_ENCODER_BCH_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"
#include "Tools/Math/Galois.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_BCH : public Factory
{
	static module::Encoder<B>* build(const std::string  type,
	                                 const int          K,
	                                 const int          N,
	                                 const Galois      &GF,
	                                 const int          n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_BCH_HPP */
