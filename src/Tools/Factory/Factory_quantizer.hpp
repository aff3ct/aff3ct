#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include "../../Quantizer/Quantizer.hpp"

#include "../params.h"

template <typename R, typename Q>
struct Factory_quantizer
{
	static Quantizer<R,Q>* build(const t_code_param &code_params, const t_channel_param &chan_params, const R& sigma);
};

#endif /* FACTORY_QUANTIZER_HPP */