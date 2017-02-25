#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "Module/Puncturer/Puncturer.hpp"
#include "Tools/params.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename Q>
struct Factory_puncturer_polar : public Factory
{
	static module::Puncturer<B,Q>* build(const parameters &params, const Frozenbits_generator<B> *fb_generator);
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
