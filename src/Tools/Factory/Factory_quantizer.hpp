#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include "Tools/params.h"

#include "Module/Quantizer/Quantizer.hpp"

template <typename R, typename Q>
struct Factory_quantizer
{
	static Quantizer<R,Q>* build(const parameters &params, const R& sigma, const int size);
};

#endif /* FACTORY_QUANTIZER_HPP */
