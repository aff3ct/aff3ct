#ifndef FACTORY_COSET_REAL_HPP
#define FACTORY_COSET_REAL_HPP

#include "Module/Coset/Coset.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename Q>
struct Factory_coset_real : public Factory
{
	static module::Coset<B,Q>* build(const parameters &params);
};
}
}

#endif /* FACTORY_COSET_REAL_HPP */
