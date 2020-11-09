#include <cstdlib>
#include <mipp.h>

#include "Module/Source/Random/Source_random_fast.hpp"

using namespace aff3ct::module;

template <typename B>
Source_random_fast<B>
::Source_random_fast(const int K, const int seed)
: Source<B>(K),
  mt19937(seed),
  mt19937_simd()
{
	const std::string name = "Source_random_fast";
	this->set_name(name);

	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

template <typename B>
Source_random_fast<B>* Source_random_fast<B>
::clone() const
{
	auto m = new Source_random_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Source_random_fast<B>
::_generate(B *U_K, const size_t frame_id)
{
	if (!mipp::isAligned(U_K))
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "'U_K' is misaligned memory.");

	const auto size = (unsigned)(this->K);

	// vectorized loop
	const auto period = mipp::nElReg<B>() * sizeof(B) * 8;
	const auto vec_loop_size = (unsigned)((size / period) * period);
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

template <typename B>
void Source_random_fast<B>
::set_seed(const int seed)
{
	mt19937.seed(seed);
	mipp::vector<int> seeds(mipp::nElReg<int>());
	for (auto i = 0; i < mipp::nElReg<int>(); i++)
		seeds[i] = mt19937.rand();
	mt19937_simd.seed(seeds.data());
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Source_random_fast<B_8>;
template class aff3ct::module::Source_random_fast<B_16>;
template class aff3ct::module::Source_random_fast<B_32>;
template class aff3ct::module::Source_random_fast<B_64>;
#else
template class aff3ct::module::Source_random_fast<B>;
#endif
// ==================================================================================== explicit template instantiation
