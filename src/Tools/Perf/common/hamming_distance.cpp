#include <limits>
#include <cmath>

#include "Tools/Noise/noise_utils.h"
#include "hamming_distance.h"

//*************************************************************************************** hamming_distance_seq(in1, in2)

template <typename B>
inline size_t aff3ct::tools::hamming_distance_seq(const B *in1, const B *in2, const unsigned size)
{
	size_t ham_dist = 0;

	for (unsigned i = 0; i < size; i++)
		ham_dist += (in1[i] != in2[i])
		            || aff3ct::tools::is_unknown_symbol<B>(in1[i])
		            || aff3ct::tools::is_unknown_symbol<B>(in2[i])
		            ? (size_t)1 : (size_t)0;

	return ham_dist;
}

template <typename B>
inline size_t hamming_distance_seq_real(const B *in1, const B *in2, const unsigned size)
{
	size_t ham_dist = 0;

	for (unsigned i = 0; i < size; i++)
		ham_dist += std::signbit(in1[i]) ^ std::signbit(in2[i])
		            || aff3ct::tools::is_unknown_symbol<B>(in1[i])
		            || aff3ct::tools::is_unknown_symbol<B>(in2[i])
		            ? (size_t)1 : (size_t)0;

	return ham_dist;
}

namespace aff3ct
{
namespace tools
{

template <>
inline size_t hamming_distance_seq(const float *in1, const float *in2, const unsigned size)
{
	return hamming_distance_seq_real(in1, in2, size);
}

template <>
inline size_t hamming_distance_seq(const double *in1, const double *in2, const unsigned size)
{
	return hamming_distance_seq_real(in1, in2, size);
}

}
}



//********************************************************************************************* hamming_distance_seq(in)

template <typename B>
inline size_t aff3ct::tools::hamming_distance_seq(const B *in, const unsigned size)
{
	size_t ham_dist = 0;

	for (unsigned i = 0; i < size; i++)
		ham_dist += in[i] || aff3ct::tools::is_unknown_symbol<B>(in[i]) ? (size_t)1 : (size_t)0;

	return ham_dist;
}


template <typename B>
inline size_t hamming_distance_seq_real(const B *in, const unsigned size)
{
	size_t ham_dist = 0;

	for (unsigned i = 0; i < size; i++)
		ham_dist += std::signbit(in[i]) || aff3ct::tools::is_unknown_symbol<B>(in[i]) ? (size_t)1 : (size_t)0;

	return ham_dist;
}


namespace aff3ct
{
namespace tools
{

template <>
inline size_t hamming_distance_seq(const float *in, const unsigned size)
{
	return hamming_distance_seq_real(in, size);
}

template <>
inline size_t hamming_distance_seq(const double *in, const unsigned size)
{
	return hamming_distance_seq_real(in, size);
}

}
}



//***************************************************************************************************** hamming_distance

#ifdef MIPP_AVX1

template <typename B>
size_t aff3ct::tools::hamming_distance(const B *in1, const B *in2, const unsigned size)
{
	return hamming_distance_seq(in1, in2, size);
}

template <typename B>
size_t aff3ct::tools::hamming_distance(const B *in, const unsigned size)
{
	return hamming_distance_seq(in, size);
}




#else // MIPP_AVX1



template <typename B>
inline mipp::Reg<B> popcnt(const mipp::Reg<B>& q_in1, const mipp::Reg<B>& q_in2)
{
	const mipp::Reg<B> zeros  = (B)0, ones = (B)1;
	auto m_in = q_in1 != q_in2;
	m_in |= aff3ct::tools::is_unknown_symbol<B>(q_in1) | aff3ct::tools::is_unknown_symbol<B>(q_in2);
	return mipp::blend(ones, zeros, m_in);
}

template <typename B>
inline mipp::Reg<B> popcnt(const mipp::Reg<B>& q_in)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	const auto m_in = (q_in != zeros) | aff3ct::tools::is_unknown_symbol<B>(q_in);
	return mipp::blend(ones, zeros, m_in);
}

template <typename B>
inline mipp::Reg<B> popcnt_real(const mipp::Reg<B>& q_in)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	const auto m_in = (q_in < zeros) | aff3ct::tools::is_unknown_symbol<B>(q_in);
	return mipp::blend(ones, zeros, m_in);
}

template <>
inline mipp::Reg<float> popcnt(const mipp::Reg<float>& q_in)
{
	return popcnt_real(q_in);
}

template <>
inline mipp::Reg<double> popcnt(const mipp::Reg<double>& q_in)
{
	return popcnt_real(q_in);
}




template <typename B>
size_t aff3ct::tools::hamming_distance(const B *in1, const B *in2, const unsigned size)
{
	mipp::Reg<B> counter = (B)0;

	const auto vec_loop_size = (size / mipp::N<B>()) * mipp::N<B>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<B>())
		counter += popcnt<B>(in1 + i, in2 + i);

	size_t ham_dist = (size_t)mipp::hadd(counter);

	ham_dist += tools::hamming_distance_seq(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}

template <typename B>
size_t aff3ct::tools::hamming_distance(const B *in, const unsigned size)
{
	mipp::Reg<B> counter = (B)0;

	const auto vec_loop_size = (size / mipp::N<B>()) * mipp::N<B>();

	for (unsigned i = 0; i < vec_loop_size; i += mipp::N<B>())
		counter += popcnt<B>(in + i);

	size_t ham_dist = (size_t)mipp::hadd(counter);

	ham_dist += tools::hamming_distance_seq(in + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}

namespace aff3ct
{
namespace tools
{
template <>
size_t hamming_distance<int16_t>(const int16_t *in1, const int16_t *in2, const unsigned size)
{
#ifdef MIPP_BW
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int16_t>() < 2 ? 0 : (size / mipp::N<int16_t>()) * mipp::N<int16_t>();
	constexpr auto stride = std::numeric_limits<int16_t>::max() * mipp::N<int16_t>();
	for (unsigned ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int16_t> counter16 = (int16_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (unsigned i = ii; i < vec_loop_size2; i += mipp::N<int16_t>())
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
size_t hamming_distance<int8_t>(const int8_t *in1, const int8_t *in2, const unsigned size)
{
#ifdef MIPP_BW
	const mipp::Reg<int8_t> zeros = (int8_t)0, ones = (int8_t)1;
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int8_t>() < 4 ? 0 : (size / mipp::N<int8_t>()) * mipp::N<int8_t>();
	constexpr auto stride = std::numeric_limits<int8_t>::max() * mipp::N<int8_t>();
	for (unsigned ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int8_t> counter8 = (int8_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (unsigned i = ii; i < vec_loop_size2; i += mipp::N<int8_t>())
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


template <>
size_t hamming_distance<int16_t>(const int16_t *in, const unsigned size)
{
#ifdef MIPP_BW
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int16_t>() < 2 ? 0 : (size / mipp::N<int16_t>()) * mipp::N<int16_t>();
	constexpr auto stride = std::numeric_limits<int16_t>::max() * mipp::N<int16_t>();
	for (unsigned ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int16_t> counter16 = (int16_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (unsigned i = ii; i < vec_loop_size2; i += mipp::N<int16_t>())
			counter16 += popcnt<int16_t>(in + i);

		counter32 += mipp::cvt<int16_t,int32_t>(counter16.low ());
		counter32 += mipp::cvt<int16_t,int32_t>(counter16.high());
	}

	size_t ham_dist = (size_t)mipp::hadd(counter32);
#else
	const auto vec_loop_size = 0;
	size_t ham_dist = 0;
#endif

	ham_dist += tools::hamming_distance_seq<int16_t>(in + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}

template <>
size_t hamming_distance<int8_t>(const int8_t *in, const unsigned size)
{
#ifdef MIPP_BW
	const mipp::Reg<int8_t> zeros = (int8_t)0, ones = (int8_t)1;
	mipp::Reg<int32_t> counter32 = (int32_t)0;

	const auto vec_loop_size = mipp::N<int8_t>() < 4 ? 0 : (size / mipp::N<int8_t>()) * mipp::N<int8_t>();
	constexpr auto stride = std::numeric_limits<int8_t>::max() * mipp::N<int8_t>();
	for (unsigned ii = 0; ii < vec_loop_size; ii += stride)
	{
		mipp::Reg<int8_t> counter8 = (int8_t)0;
		const auto vec_loop_size2 = std::min(vec_loop_size, ii + stride);
		for (unsigned i = ii; i < vec_loop_size2; i += mipp::N<int8_t>())
			counter8 += popcnt<int8_t>(in + i);

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

	ham_dist += tools::hamming_distance_seq<int8_t>(in + vec_loop_size, size - vec_loop_size);

	return ham_dist;
}
}
}

#endif // #ifdef MIPP_AVX

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance<B_8 >(const B_8*,  const B_8*, const unsigned);
template size_t aff3ct::tools::hamming_distance<B_16>(const B_16*, const B_16*, const unsigned);
template size_t aff3ct::tools::hamming_distance<B_32>(const B_32*, const B_32*, const unsigned);
template size_t aff3ct::tools::hamming_distance<B_64>(const B_64*, const B_64*, const unsigned);
#else
template size_t aff3ct::tools::hamming_distance<B>(const B*, const B*, const unsigned);
#endif
template size_t aff3ct::tools::hamming_distance<float>(const float*, const float*, const unsigned);
template size_t aff3ct::tools::hamming_distance<double>(const double*, const double*, const unsigned);

#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance_seq<B_8 >(const B_8*,  const B_8*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_16>(const B_16*, const B_16*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_32>(const B_32*, const B_32*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_64>(const B_64*, const B_64*, const unsigned);
#else
template size_t aff3ct::tools::hamming_distance_seq<B>(const B*, const B*, const unsigned);
#endif
template size_t aff3ct::tools::hamming_distance_seq<float>(const float*, const float*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<double>(const double*, const double*, const unsigned);

#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance<B_8 >(const B_8*,  const unsigned);
template size_t aff3ct::tools::hamming_distance<B_16>(const B_16*, const unsigned);
template size_t aff3ct::tools::hamming_distance<B_32>(const B_32*, const unsigned);
template size_t aff3ct::tools::hamming_distance<B_64>(const B_64*, const unsigned);
#else
template size_t aff3ct::tools::hamming_distance<B>(const B*, const unsigned);
#endif
template size_t aff3ct::tools::hamming_distance<float>(const float*, const unsigned);
template size_t aff3ct::tools::hamming_distance<double>(const double*, const unsigned);


#ifdef MULTI_PREC
template size_t aff3ct::tools::hamming_distance_seq<B_8 >(const B_8*,  const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_16>(const B_16*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_32>(const B_32*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<B_64>(const B_64*, const unsigned);
#else
template size_t aff3ct::tools::hamming_distance_seq<B>(const B*, const unsigned);
#endif
template size_t aff3ct::tools::hamming_distance_seq<float>(const float*, const unsigned);
template size_t aff3ct::tools::hamming_distance_seq<double>(const double*, const unsigned);


// ==================================================================================== explicit template instantiation
