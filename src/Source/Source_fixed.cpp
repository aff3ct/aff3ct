#include "Source_fixed.hpp"

template <typename B>
Source_fixed<B>
::Source_fixed() 
{
}

template <typename B>
Source_fixed<B>
::~Source_fixed()
{
}

template <typename B>
void Source_fixed<B>
::generate(mipp::vector<B>& U_K)
{
	auto size = (int)U_K.size();
	// Reads Vector Stored in HPP as vector U_k
	for (auto i = 0; i < size; i++)
		U_K[i] = this->fixed_word[i];
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Source_fixed<B_8>;
template class Source_fixed<B_16>;
template class Source_fixed<B_32>;
template class Source_fixed<B_64>;
#else
template class Source_fixed<B>;
#endif
// ==================================================================================== explicit template instantiation