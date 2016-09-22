#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "Module/Decoder/Decoder.hpp"
#include "Module/Decoder/SISO.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

template <typename B, typename R>
struct Factory_decoder_repetition : public Factory
{
	static Decoder<B,R>* build(const parameters &params);
};

#endif /* FACTORY_DECODER_REPETITION_HPP */
