#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#define FACTORY_FROZENBITS_GENERATOR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_frozenbits_generator : public Factory
{
	static Frozenbits_generator<B>* build(const std::string type,
	                                      const int         K,
	                                      const int         N,
	                                      const float       sigma,
	                                      const std::string path_fb = "",
	                                      const std::string path_pb = "");
};
}
}

#endif /* FACTORY_FROZENBITS_GENERATOR_HPP */
