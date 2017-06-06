#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include <string>

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_LDPC : public Factory
{
	static module::Encoder_LDPC<B>* build(const std::string type,
	                                      const int         K,
	                                      const int         N,
	                                      const std::string path     = "",
	                                      const int         n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
