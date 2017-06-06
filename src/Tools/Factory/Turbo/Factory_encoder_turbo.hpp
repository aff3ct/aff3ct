#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_turbo : public Factory
{
	static module::Encoder<B>* build(const std::string               type,
	                                 const int                       K,
	                                 const int                       N,
	                                 const module::Interleaver<int> &itl,
	                                       module::Encoder_sys<B>   *enc_n,
	                                       module::Encoder_sys<B>   *enc_i    = nullptr,
	                                 const bool                      buffered = true,
	                                 const int                       n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_HPP */
