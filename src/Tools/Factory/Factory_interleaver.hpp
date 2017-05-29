#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = int>
struct Factory_interleaver : public Factory
{
	static module::Interleaver<T>* build(const std::string type,
	                                     const int         size,
	                                     const std::string path     = "",
	                                     const bool        uniform  = false,
	                                     const int         n_cols   = 1,
	                                     const int         seed     = 0,
	                                     const int         n_frames = 1);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
