#ifndef COMMON_H_
#define COMMON_H_

#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
inline void hard_decide(const R *in, B *out, const int size)
{
	const auto vec_loop_size = (size / mipp::nElReg<R>()) * mipp::nElReg<R>();
	if (mipp::isAligned(in) && mipp::isAligned(out))
	{
		for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
		{
			const auto r_in  = mipp::Reg<R>(&in[i]);
			const auto r_out = mipp::cast<R,B>(r_in) >> (sizeof(B) * 8 - 1);
			r_out.store(&out[i]);
		}
	}
	else
	{
		for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
		{
			mipp::Reg<R> r_in;
			r_in.loadu(&in[i]);
			const auto r_out = mipp::cast<R,B>(r_in) >> (sizeof(B) * 8 - 1);
			r_out.storeu(&out[i]);
		}
	}
	for (auto i = vec_loop_size; i < size; i++)
		out[i] = in[i] < 0;
}

template <typename B = int>
inline size_t hamming_distance(const B *in1, const B *in2, const int size)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	mipp::Reg<B> counter = (B)0;

	const auto vec_loop_size = (size / mipp::nElReg<B>()) * mipp::nElReg<B>();

	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<B>())
	{
		const auto r_in1 = mipp::Reg<B>(&in1[i]);
		const auto r_in2 = mipp::Reg<B>(&in2[i]);
		const auto m_in1 = r_in1 != zeros;
		const auto m_in2 = r_in2 != zeros;
		counter += mipp::blend(ones, zeros, m_in1 ^ m_in2);
	}

	size_t ham_dist = mipp::hadd(counter);

	for (auto i = vec_loop_size; i < size; i++)
		ham_dist += (!in1[i] != !in2[i])? 1 : 0;

	return ham_dist;
}

}
}

#endif /* COMMON_H_ */