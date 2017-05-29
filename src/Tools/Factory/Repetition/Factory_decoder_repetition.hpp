#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_repetition : public Factory
{
	static module::Decoder<B,R>* build(const std::string type,
	                                   const std::string implem,
	                                   const int         K,
	                                   const int         N,
	                                   const bool        buffered = true,
	                                   const int         n_frames = 1);
};
}
}

#endif /* FACTORY_DECODER_REPETITION_HPP */
