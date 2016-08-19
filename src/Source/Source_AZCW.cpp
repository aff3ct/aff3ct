#include "Source_AZCW.hpp"

template <typename B>
Source_AZCW<B>
::Source_AZCW(const int K, const int n_frames, const std::string name)
: Source<B>(K, n_frames, name.c_str())
{
}

template <typename B>
Source_AZCW<B>
::~Source_AZCW()
{
}

template <typename B>
void Source_AZCW<B>
::generate(mipp::vector<B>& U_K)
{
	std::fill(U_K.begin(), U_K.end(), 0);
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Source_AZCW<B_8>;
template class Source_AZCW<B_16>;
template class Source_AZCW<B_32>;
template class Source_AZCW<B_64>;
#else
template class Source_AZCW<B>;
#endif
// ==================================================================================== explicit template instantiation