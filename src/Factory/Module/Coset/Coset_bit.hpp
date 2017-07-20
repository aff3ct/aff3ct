#ifndef FACTORY_COSET_BIT_HPP
#define FACTORY_COSET_BIT_HPP

#include <string>

#include "Module/Coset/Coset.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Coset_bit : public Factory
{
	template <typename B = int>
	static module::Coset<B,B>* build(const std::string type,
	                                 const int         K,
	                                 const int         n_frames = 1);
};
}
}

#endif /* FACTORY_COSET_BIT_HPP */
