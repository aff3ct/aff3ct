#ifndef HARD_DECISION_H_
#define HARD_DECISION_H_

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
}
}

#endif /* HARD_DECISION_H_ */
