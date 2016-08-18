#include "Source_random.hpp"

template <typename B>
Source_random<B>
::Source_random(const int seed, const std::string name)
: Source<B>(name.c_str()),
  rd_engine(seed), 
  uniform_dist(0, 1) 
{
}

template <typename B>
Source_random<B>
::~Source_random()
{
}

template <typename B>
void Source_random<B>
::generate(mipp::vector<B>& U_K)
{
	auto size = (int)U_K.size();

	// generate a random k bits vector U_k
	for (auto i = 0; i < size; i++)
		U_K[i] = this->uniform_dist(this->rd_engine);
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Source_random<B_8>;
template class Source_random<B_16>;
template class Source_random<B_32>;
template class Source_random<B_64>;
#else
template class Source_random<B>;
#endif
// ==================================================================================== explicit template instantiation