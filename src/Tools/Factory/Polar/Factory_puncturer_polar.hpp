#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Puncturer/Puncturer.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_puncturer_polar : public Factory
{
	static module::Puncturer<B,Q>* build(const std::string              type,
	                                     const int                      K,
	                                     const int                      N,
	                                     const Frozenbits_generator<B> *fb_generator,
	                                     const int                      n_frames = 1);
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
