#include <limits>

#include "hard_decide.h"

template <typename B, typename Q>
void aff3ct::tools::hard_decide(const Q *in, B *out, const int size)
{
	const auto vec_loop_size = (size / mipp::nElReg<Q>()) * mipp::nElReg<Q>();
	if (mipp::isAligned(in) && mipp::isAligned(out))
	{
		for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			const auto q_in  = mipp::Reg<Q>(&in[i]);
			const auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out.store(&out[i]);
		}
	}
	else
	{
		for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			mipp::Reg<Q> q_in;
			q_in.loadu(&in[i]);
			const auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out.storeu(&out[i]);
		}
	}
	for (auto i = vec_loop_size; i < size; i++)
		out[i] = in[i] < 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template void aff3ct::tools::hard_decide<B_8,  Q_8 >(const Q_8*,  B_8*,  const int);
template void aff3ct::tools::hard_decide<B_16, Q_16>(const Q_16*, B_16*, const int);
template void aff3ct::tools::hard_decide<B_32, Q_32>(const Q_32*, B_32*, const int);
template void aff3ct::tools::hard_decide<B_64, Q_64>(const Q_64*, B_64*, const int);
#else
template void aff3ct::tools::hard_decide<B, Q>(const Q*, B*, const int);
#endif
// ==================================================================================== explicit template instantiation
