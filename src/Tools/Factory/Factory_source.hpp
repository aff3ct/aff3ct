#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_source : public Factory
{
	static module::Source<B>* build(const std::string type,
	                                const int         K,
	                                const std::string path     = "",
	                                const int         seed     = 0,
	                                const int         n_frames = 1);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
