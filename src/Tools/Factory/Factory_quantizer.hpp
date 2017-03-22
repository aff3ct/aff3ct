#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include "Module/Quantizer/Quantizer.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, typename Q = R>
struct Factory_quantizer : public Factory
{
	static module::Quantizer<R,Q>* build(const parameters &params, const float& sigma, const int size);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
