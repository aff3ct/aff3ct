#ifndef FACTORY_DECODER_TURBO_HPP
#define FACTORY_DECODER_TURBO_HPP

#include <string>

#include "Module/Decoder/Turbo/Decoder_turbo.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Module/Interleaver/Interleaver.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_turbo : public Factory
{
	static module::Decoder_turbo<B,R>* build(const std::string               type,
	                                         const std::string               implem,
	                                         const int                       K,
	                                         const int                       N,
	                                         const int                       n_ite,
	                                         const module::Interleaver<int> &itl,
	                                               module::SISO<R>          &siso_n,
	                                               module::SISO<R>          &siso_i,
	                                         const bool                      buffered = true);
};
}
}

#endif /* FACTORY_DECODER_TURBO_HPP */
