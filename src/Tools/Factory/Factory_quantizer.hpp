#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>

#include "Module/Quantizer/Quantizer.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, typename Q = R>
struct Factory_quantizer : public Factory
{
	static module::Quantizer<R,Q>* build(const std::string type,
	                                     const int         size,
	                                     const int         n_decimals,
	                                     const int         n_bits,
	                                     const float       sigma    = 0.f,
	                                     const float       range    = 0.f,
	                                     const int         n_frames = 1);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
