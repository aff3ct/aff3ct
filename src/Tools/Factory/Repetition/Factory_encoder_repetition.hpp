#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>

#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_repetition : public Factory
{
	static module::Encoder_sys<B>* build(const std::string type,
	                                     const int         K,
	                                     const int         N,
	                                     const bool        buffered = true,
	                                     const int         n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
