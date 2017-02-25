#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include "Module/Interleaver/Interleaver.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
struct Factory_interleaver : public Factory
{
	static module::Interleaver<T>* build(const parameters &params, const int &size, const int seed = -1);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
