#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include "Module/Quantizer/Quantizer.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename R, typename Q>
struct Factory_quantizer : public Factory
{
	static Quantizer<R,Q>* build(const parameters &params, const R& sigma, const int size);
};

#endif /* FACTORY_QUANTIZER_HPP */
