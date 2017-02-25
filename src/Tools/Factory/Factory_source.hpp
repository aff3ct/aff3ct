#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include "Module/Source/Source.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B>
struct Factory_source : public Factory
{
	static module::Source<B>* build(const parameters &params, const int seed = 0);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
