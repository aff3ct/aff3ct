#ifndef FACTORY_DECODER_BCH_HPP
#define FACTORY_DECODER_BCH_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/params.h"
#include "Tools/Math/Galois.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_decoder_BCH : public Factory
{
	static module::Decoder<B,R>* build(const parameters &params, const Galois &GF);
};
}
}

#endif /* FACTORY_DECODER_BCH_HPP */
