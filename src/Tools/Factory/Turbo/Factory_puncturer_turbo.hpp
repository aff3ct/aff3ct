#ifndef FACTORY_PUNCTURER_TURBO_HPP
#define FACTORY_PUNCTURER_TURBO_HPP

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_puncturer_turbo : public Factory
{
	static module::Puncturer<B,Q>* build(const std::string type,
	                                     const int         K,
	                                     const int         N,
	                                     const int         tail_length,
	                                     const std::string pattern,
	                                     const bool        buffered = true,
	                                     const int         n_frames = 1);
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_HPP */
