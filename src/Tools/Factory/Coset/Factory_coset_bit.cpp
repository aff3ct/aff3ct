#include "Module/Coset/Bit/Coset_bit.hpp"

#include "Factory_coset_bit.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Coset<B,B>* Factory_coset_bit<B>
::build(const parameters &params)
{
	Coset<B,B> *coset = nullptr;

	// build the coset
	coset = new Coset_bit<B,B>(params.code.K);

	return coset;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct Factory_coset_bit<B_8 >;
template struct Factory_coset_bit<B_16>;
template struct Factory_coset_bit<B_32>;
template struct Factory_coset_bit<B_64>;
#else
template struct Factory_coset_bit<B>;
#endif
// ==================================================================================== explicit template instantiation
