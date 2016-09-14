#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include "Module/Encoder/Encoder_sys.hpp"

#include "Tools/params.h"

template <typename B>
struct Factory_encoder_repetition
{
	static Encoder_sys<B>* build(const parameters &params);
};

#endif /* FACTORY_ENCODER_REPETITION_HPP */
