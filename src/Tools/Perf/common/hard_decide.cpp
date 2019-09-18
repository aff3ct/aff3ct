#include <mipp.h>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Perf/common/hard_decide.h"

template <typename B, typename Q>
void aff3ct::tools::hard_decide_seq(const Q *in, B *out, const unsigned size)
{
	for (unsigned i = 0; i < size; i++)
		out[i] = in[i] < 0;
}

template <typename B, typename Q>
void aff3ct::tools::hard_decide(const Q *in, B *out, const unsigned size)
{
	const auto vec_loop_size = (size / (unsigned)mipp::nElReg<Q>()) * (unsigned)mipp::nElReg<Q>();
	if (mipp::isAligned(in) && mipp::isAligned(out))
	{
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			const auto q_in  = mipp::Reg<Q>(&in[i]);
			const auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out.store(&out[i]);
		}
	}
	else
	{
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			mipp::Reg<Q> q_in;
			q_in.loadu(&in[i]);
			const auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out.storeu(&out[i]);
		}
	}

	tools::hard_decide_seq(in + vec_loop_size, out + vec_loop_size, size - vec_loop_size);
}



template <typename B, typename Q>
void aff3ct::tools::hard_decide_unk_seq(const Q *in, B *out, const unsigned size)
{
	for (unsigned i = 0; i < size; i++)
		out[i] = tools::is_unknown_llr<Q>(in[i]) ? tools::unknown_symbol_val<B>() : (in[i] < 0);
}

template <typename B, typename Q>
void aff3ct::tools::hard_decide_unk(const Q *in, B *out, const unsigned size)
{
	const mipp::Reg<B> r_unks = tools::unknown_symbol_val<B>();

	const auto vec_loop_size = (size / (unsigned)mipp::nElReg<Q>()) * (unsigned)mipp::nElReg<Q>();
	if (mipp::isAligned(in) && mipp::isAligned(out))
	{
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			const auto q_in  = mipp::Reg<Q>(&in[i]);
			const auto m_unk = tools::is_unknown_llr<Q>(q_in);
			auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out = mipp::blend(r_unks, q_out, m_unk);
			q_out.store(&out[i]);
		}
	}
	else
	{
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			mipp::Reg<Q> q_in;
			q_in.loadu(&in[i]);
			const auto m_unk = tools::is_unknown_llr<Q>(q_in);
			auto q_out = mipp::cast<Q,B>(q_in) >> (sizeof(B) * 8 - 1);
			q_out = mipp::blend(r_unks, q_out, m_unk);
			q_out.storeu(&out[i]);
		}
	}

	tools::hard_decide_seq(in + vec_loop_size, out + vec_loop_size, size - vec_loop_size);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template void aff3ct::tools::hard_decide<B_8,  Q_8 >(const Q_8*,  B_8*,  const unsigned);
template void aff3ct::tools::hard_decide<B_16, Q_16>(const Q_16*, B_16*, const unsigned);
template void aff3ct::tools::hard_decide<B_32, Q_32>(const Q_32*, B_32*, const unsigned);
template void aff3ct::tools::hard_decide<B_64, Q_64>(const Q_64*, B_64*, const unsigned);

template void aff3ct::tools::hard_decide_unk<B_8,  Q_8 >(const Q_8*,  B_8*,  const unsigned);
template void aff3ct::tools::hard_decide_unk<B_16, Q_16>(const Q_16*, B_16*, const unsigned);
template void aff3ct::tools::hard_decide_unk<B_32, Q_32>(const Q_32*, B_32*, const unsigned);
template void aff3ct::tools::hard_decide_unk<B_64, Q_64>(const Q_64*, B_64*, const unsigned);

template void aff3ct::tools::hard_decide_seq<B_8,  Q_8 >(const Q_8*,  B_8*,  const unsigned);
template void aff3ct::tools::hard_decide_seq<B_16, Q_16>(const Q_16*, B_16*, const unsigned);
template void aff3ct::tools::hard_decide_seq<B_32, Q_32>(const Q_32*, B_32*, const unsigned);
template void aff3ct::tools::hard_decide_seq<B_64, Q_64>(const Q_64*, B_64*, const unsigned);

template void aff3ct::tools::hard_decide_unk_seq<B_8,  Q_8 >(const Q_8*,  B_8*,  const unsigned);
template void aff3ct::tools::hard_decide_unk_seq<B_16, Q_16>(const Q_16*, B_16*, const unsigned);
template void aff3ct::tools::hard_decide_unk_seq<B_32, Q_32>(const Q_32*, B_32*, const unsigned);
template void aff3ct::tools::hard_decide_unk_seq<B_64, Q_64>(const Q_64*, B_64*, const unsigned);
#else
template void aff3ct::tools::hard_decide_seq    <B, Q>(const Q*, B*, const unsigned);
template void aff3ct::tools::hard_decide_unk_seq<B, Q>(const Q*, B*, const unsigned);

template void aff3ct::tools::hard_decide    <B, Q>(const Q*, B*, const unsigned);
template void aff3ct::tools::hard_decide_unk<B, Q>(const Q*, B*, const unsigned);
#endif
// ==================================================================================== explicit template instantiation
