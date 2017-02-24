#ifndef FACTORY_COSET_BIT_HPP
#define FACTORY_COSET_BIT_HPP

#include "Module/Coset/Coset.hpp"
#include "Tools/params.h"

#include "../Factory.hpp"

namespace aff3ct
{
template <typename B>
struct Factory_coset_bit : public Factory
{
	static Coset<B,B>* build(const parameters &params);
};
}

#endif /* FACTORY_COSET_BIT_HPP */
