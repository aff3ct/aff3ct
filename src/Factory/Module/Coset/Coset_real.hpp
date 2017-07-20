#ifndef FACTORY_COSET_REAL_HPP
#define FACTORY_COSET_REAL_HPP

#include <string>

#include "Module/Coset/Coset.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Coset_real : public Factory
{
	template <typename B = int, typename Q = float>
	static module::Coset<B,Q>* build(const std::string type,
	                                 const int         N,
	                                 const int         n_frames = 1);
};
}
}

#endif /* FACTORY_COSET_REAL_HPP */
