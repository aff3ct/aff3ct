#include <cstdlib>

#include "Source_random_fast.hpp"

template <typename B>
Source_random_fast<B>
::Source_random_fast(const int seed, const std::string name)
: Source<B>(name.c_str()),
  mt19937(seed),
  mt19937_simd()
{
	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename B>
Source_random_fast<B>
::~Source_random_fast()
{
}

template <typename B>
void Source_random_fast<B>
::generate(mipp::vector<B>& U_K)
{
	const auto size = U_K.size();

	// vectorized loop
	const auto period = mipp::nElReg<B>() * sizeof(B) * 8; 
	const auto vec_loop_size = (size / period) * period;
	for (unsigned i = 0; i < vec_loop_size; i += period)
	{
		mipp::Reg<int> randoms_s32 = mt19937_simd.rand_s32();
		mipp::Reg<B>   randoms     = randoms_s32.r;

		for (unsigned j = 0; j < (sizeof(B) * 8); j++)
		{
			auto r = randoms & 0x1;
			r.store(&U_K[i + j * mipp::nElReg<B>()]);
			randoms >>= 1;
		}
	}

	// remaining scalar operations
	for (unsigned i = vec_loop_size; i < size; i += sizeof(B) * 8)
	{
		auto randoms = mt19937.rand_u32();
		unsigned j = 0;
		while ((j < sizeof(B) * 8) && (i + j < size))
		{
			auto r = randoms & 0x1;
			U_K[i + j] = r;
			randoms >>= 1;
			j++;
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Source_random_fast<B_8>;
template class Source_random_fast<B_16>;
template class Source_random_fast<B_32>;
template class Source_random_fast<B_64>;
#else
template class Source_random_fast<B>;
#endif
// ==================================================================================== explicit template instantiation