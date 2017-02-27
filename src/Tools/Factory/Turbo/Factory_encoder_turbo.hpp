#ifndef FACTORY_ENCODER_TURBO_HPP
#define FACTORY_ENCODER_TURBO_HPP

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Encoder/Encoder_sys.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_turbo : public Factory
{
	static module::Encoder<B>* build(const parameters                  &params,
	                                       module::Interleaver<short>  *interleaver,
	                                       module::Encoder_sys<B>      *sub_encoder_n,
	                                       module::Encoder_sys<B>      *sub_encoder_i,
	                                 const int                          n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_TURBO_HPP */
