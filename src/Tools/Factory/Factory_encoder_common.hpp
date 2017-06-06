#ifndef FACTORY_ENCODER_COMMON_HPP
#define FACTORY_ENCODER_COMMON_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_common : public Factory
{
	static module::Encoder<B>* build(const std::string type,
	                                 const int         K,
	                                 const int         N,
	                                 const std::string path     = "",
	                                 const int         seed     = 0,
	                                 const int         n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_COMMON_HPP */
