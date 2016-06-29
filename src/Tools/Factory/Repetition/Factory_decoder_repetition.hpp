#ifndef FACTORY_DECODER_REPETITION_HPP
#define FACTORY_DECODER_REPETITION_HPP

#include <string>

#include "../../../Decoder/Decoder.hpp"
#include "../../../Decoder/SISO.hpp"

#include "../../params.h"

template <typename B, typename R>
struct Factory_decoder_repetition
{
	static Decoder<B,R>* build(const t_code_param    &code_params,
	                           const t_encoder_param &enco_params,
	                           const t_decoder_param &deco_params);
};

#endif /* FACTORY_DECODER_REPETITION_HPP */