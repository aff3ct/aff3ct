#include <limits>

#include "common.h"

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



template <typename B>
inline size_t aff3ct::tools::hamming_distance_seq(const B *in1, const B *in2, const int size)
{
	size_t ham_dist = 0;

	for (auto i = 0; i < size; i++)
		ham_dist += (!in1[i] != !in2[i])? (size_t)1 : (size_t)0;

	return ham_dist;
}

template <typename B>
inline mipp::Reg<B> popcnt(const mipp::Reg<B>& q_in1, const mipp::Reg<B>& q_in2)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	const auto m_in1 = q_in1 != zeros;
	const auto m_in2 = q_in2 != zeros;
	return mipp::blend(ones, zeros, m_in1 ^ m_in2);
}


template <typename B>
size_t aff3ct::tools::hamming_distance(const B *in1, const B *in2, const int size)
{
	mipp::Reg<B> counter = (B)0;

	const auto vec_loop_size = (size / mipp::N<B>()) * mipp::N<B>();

	for (auto i = 0; i < vec_loop_size; i += mipp::N<B>())
		counter += popcnt<B>(in1 + i, in2 + i);

	size_t ham_dist = mipp::hadd(counter);

	ham_dist += tools::hamming_distance_seq(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}

namespace aff3ct
{
namespace tools
{
template <>
size_t hamming_distance<int16_t>(const int16_t *in1, const int16_t *in2, const int size)
{
#ifdef MIPP_BW
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int16_t>() < 2 ? 0 : (size / mipp::N<int16_t>()) * mipp::N<int16_t>();
	constexpr auto stride = std::numeric_limits<int16_t>::max() * mipp::N<int16_t>();
	for (auto ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int16_t> counter16 = (int16_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (auto i = ii; i < vec_loop_size2; i += mipp::N<int16_t>())
			counter16 += popcnt<int16_t>(in1 + i, in2 + i);

		counter32 += mipp::cvt<int16_t,int32_t>(counter16.low ());
		counter32 += mipp::cvt<int16_t,int32_t>(counter16.high());
	}

	size_t ham_dist = (size_t)mipp::hadd(counter32);
#else
	const auto vec_loop_size = 0;
	size_t ham_dist = 0;
#endif

	ham_dist += tools::hamming_distance_seq<int16_t>(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}

template <>
size_t hamming_distance<int8_t>(const int8_t *in1, const int8_t *in2, const int size)
{
#ifdef MIPP_BW
	const mipp::Reg<int8_t> zeros = (int8_t)0, ones = (int8_t)1;
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int8_t>() < 4 ? 0 : (size / mipp::N<int8_t>()) * mipp::N<int8_t>();
	constexpr auto stride = std::numeric_limits<int8_t>::max() * mipp::N<int8_t>();
	for (auto ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int8_t> counter8 = (int8_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (auto i = ii; i < vec_loop_size2; i += mipp::N<int8_t>())
			counter8 += popcnt<int8_t>(in1 + i, in2 + i);

		const auto low = mipp::cvt<int8_t,int16_t>(counter8.low());
		counter32 += mipp::cvt<int16_t,int32_t>(low.low ());
		counter32 += mipp::cvt<int16_t,int32_t>(low.high());

		const auto high = mipp::cvt<int8_t,int16_t>(counter8.high());
		counter32 += mipp::cvt<int16_t,int32_t>(high.low ());
		counter32 += mipp::cvt<int16_t,int32_t>(high.high());
	}

	size_t ham_dist = (size_t)mipp::hadd(counter32);

#else
	const auto vec_loop_size = 0;
	size_t ham_dist = 0;
#endif

	ham_dist += tools::hamming_distance_seq<int8_t>(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}
}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance<B_8 >(const B_8*,  const B_8*, const int);
template size_t aff3ct::tools::hamming_distance<B_16>(const B_16*, const B_16*, const int);
template size_t aff3ct::tools::hamming_distance<B_32>(const B_32*, const B_32*, const int);
template size_t aff3ct::tools::hamming_distance<B_64>(const B_64*, const B_64*, const int);
#else
template size_t aff3ct::tools::hamming_distance<B>(const B*, const B*, const int);
#endif

#ifdef MULTI_PREC
template void aff3ct::tools::hard_decide<B_8,  Q_8 >(const Q_8*,  B_8*,  const int);
template void aff3ct::tools::hard_decide<B_16, Q_16>(const Q_16*, B_16*, const int);
template void aff3ct::tools::hard_decide<B_32, Q_32>(const Q_32*, B_32*, const int);
template void aff3ct::tools::hard_decide<B_64, Q_64>(const Q_64*, B_64*, const int);
#else
template void aff3ct::tools::hard_decide<B, Q>(const Q*, B*, const int);
#endif

#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance_seq<B_8 >(const B_8*,  const B_8*, const int);
template size_t aff3ct::tools::hamming_distance_seq<B_16>(const B_16*, const B_16*, const int);
template size_t aff3ct::tools::hamming_distance_seq<B_32>(const B_32*, const B_32*, const int);
template size_t aff3ct::tools::hamming_distance_seq<B_64>(const B_64*, const B_64*, const int);
#else
template size_t aff3ct::tools::hamming_distance_seq<B>(const B*, const B*, const int);
#endif

// ==================================================================================== explicit template instantiation
