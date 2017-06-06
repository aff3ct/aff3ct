#ifndef FACTORY_COSET_BIT_HPP
#define FACTORY_COSET_BIT_HPP

#include <string>

#include "Module/Coset/Coset.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_coset_bit : public Factory
{
	static module::Coset<B,B>* build(const std::string type,
	                                 const int         K,
	                                 const int         n_frames = 1);
};
}
}

#endif /* FACTORY_COSET_BIT_HPP */
