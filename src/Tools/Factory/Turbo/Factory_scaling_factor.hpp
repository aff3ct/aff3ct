#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include <string>

#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
struct Factory_scaling_factor : public Factory
{
	static Scaling_factor<R>* build(const std::string type,
	                                const int         K,
	                                const int         n_ite,
	                                const float       cst = 0.75f);
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
