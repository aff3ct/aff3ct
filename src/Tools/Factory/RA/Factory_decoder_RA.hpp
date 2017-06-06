#ifndef FACTORY_DECODER_RA_HPP
#define FACTORY_DECODER_RA_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_RA : public Factory
{
	static module::Decoder<B,R>* build(const std::string               type,
	                                   const std::string               implem,
	                                   const int                       K,
	                                   const int                       N,
	                                   const module::Interleaver<int> &itl,
	                                   const int                       n_ite,
	                                   const int                       n_frames = 1);
};
}
}

#endif /* FACTORY_DECODER_RA_HPP */
