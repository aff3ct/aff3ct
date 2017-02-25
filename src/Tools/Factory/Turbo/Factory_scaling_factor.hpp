#ifndef FACTORY_SCALING_FACTOR_HPP
#define FACTORY_SCALING_FACTOR_HPP

#include "Tools/params.h"
#include "Tools/Code/Turbo/Scaling_factor/Scaling_factor.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
struct Factory_scaling_factor : public Factory
{
	static Scaling_factor<R>* build(const parameters &params);
};
}
}

#endif /* FACTORY_SCALING_FACTOR_HPP */
