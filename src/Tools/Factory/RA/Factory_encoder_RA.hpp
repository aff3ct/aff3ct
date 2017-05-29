#ifndef FACTORY_ENCODER_RA_HPP
#define FACTORY_ENCODER_RA_HPP

#include <string>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_RA : public Factory
{
	static module::Encoder<B>* build(const std::string               type,
	                                 const int                       K,
	                                 const int                       N,
	                                 const module::Interleaver<int> &itl,
	                                 const int                       n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_RA_HPP */
