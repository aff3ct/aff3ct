#include "Source_AZCW.hpp"

using namespace aff3ct::module;

template <typename B>
Source_AZCW<B>
::Source_AZCW(const int K, const int n_frames, const std::string name)
: Source<B>(K, n_frames, name)
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
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Source_AZCW<B_8>;
template class aff3ct::module::Source_AZCW<B_16>;
template class aff3ct::module::Source_AZCW<B_32>;
template class aff3ct::module::Source_AZCW<B_64>;
#else
template class aff3ct::module::Source_AZCW<B>;
#endif
// ==================================================================================== explicit template instantiation
