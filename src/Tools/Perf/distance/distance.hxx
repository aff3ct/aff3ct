#include <algorithm>
#include <limits>
#include <mipp.h>

#include "Tools/Perf/distance/distance.h"

namespace aff3ct
{
namespace tools
{

template <typename B, class Fdiffcnt>
size_t distance_seq(const B *in1, const B *in2, const unsigned size)
{
	size_t dist = 0;

	for (unsigned i = 0; i < size; i++)
		dist += (size_t)Fdiffcnt::apply(in1[i], in2[i]);

	return dist;
}

template <typename B, class Fdiffcnt>
size_t distance_seq(const B *in, const unsigned size)
{
	size_t dist = 0;

	for (unsigned i = 0; i < size; i++)
		dist += (size_t)Fdiffcnt::apply(in[i]);

	return dist;
}


#ifdef MIPP_AVX1

template <typename T, class Fdiffcnt>
size_t distance(const T *in1, const T *in2, const unsigned size)
{
	return distance_seq<T,Fdiffcnt>(in1, in2, size);
}

template <typename T, class Fdiffcnt>
size_t distance(const T *in, const unsigned size)
{
	return distance_seq<T,Fdiffcnt>(in, size);
}


#else // MIPP_AVX1


template <typename T, class Fdiffcnt>
struct Distance // for T on 32 or 64 bits
{
	static size_t apply(const T *in1, const T *in2, const unsigned size)
	{
		mipp::Reg<T> counter = (T)0;

		const auto vec_loop_size = (size / mipp::N<T>()) * mipp::N<T>();

		for (unsigned i = 0; i < vec_loop_size; i += mipp::N<T>())
			counter += Fdiffcnt::apply(mipp::Reg<T>(in1 + i), mipp::Reg<T>(in2 + i));

		auto dist = (size_t)mipp::hadd(counter);

		dist += distance_seq<T,Fdiffcnt>(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

		return dist;
	}

	static size_t apply(const T *in, const unsigned size)
	{
		mipp::Reg<T> counter = (T)0;

		const auto vec_loop_size = (size / mipp::N<T>()) * mipp::N<T>();

		for (unsigned i = 0; i < vec_loop_size; i += mipp::N<T>())
			counter += Fdiffcnt::apply(mipp::Reg<T>(in + i));

		auto dist = (size_t)mipp::hadd(counter);

		dist += distance_seq<T,Fdiffcnt>(in + vec_loop_size, size - vec_loop_size);

		return dist;
	}
};


template <class Fdiffcnt>
struct Distance<int16_t, Fdiffcnt>
{
	static size_t apply(const int16_t *in1, const int16_t *in2, const unsigned size)
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
				counter16 += Fdiffcnt::apply(mipp::Reg<int16_t>(in1 + i), mipp::Reg<int16_t>(in2 + i));

			counter32 += mipp::cvt<int16_t,int32_t>(counter16.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(counter16.high());
		}

		auto dist = (size_t)mipp::hadd(counter32);
#else
		const auto vec_loop_size = 0;
		size_t dist = 0;
#endif

		dist += distance_seq<int16_t,Fdiffcnt>(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

		return dist;
	}

	static size_t apply(const int16_t *in, const unsigned size)
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
				counter16 += Fdiffcnt::apply(mipp::Reg<int16_t>(in + i));

			counter32 += mipp::cvt<int16_t,int32_t>(counter16.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(counter16.high());
		}

		auto dist = (size_t)mipp::hadd(counter32);
#else
		const auto vec_loop_size = 0;
		size_t dist = 0;
#endif

		dist += distance_seq<int16_t,Fdiffcnt>(in + vec_loop_size, size - vec_loop_size);

		return dist;
	}
};


template <class Fdiffcnt>
struct Distance<int8_t, Fdiffcnt>
{
	static size_t apply(const int8_t *in1, const int8_t *in2, const unsigned size)
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
				counter8 += Fdiffcnt::apply(mipp::Reg<int8_t>(in1 + i), mipp::Reg<int8_t>(in2 + i));

			const auto low = mipp::cvt<int8_t,int16_t>(counter8.low());
			counter32 += mipp::cvt<int16_t,int32_t>(low.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(low.high());

			const auto high = mipp::cvt<int8_t,int16_t>(counter8.high());
			counter32 += mipp::cvt<int16_t,int32_t>(high.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(high.high());
		}

		auto dist = (size_t)mipp::hadd(counter32);

#else
		const auto vec_loop_size = 0;
		size_t dist = 0;
#endif

		dist += distance_seq<int8_t,Fdiffcnt>(in1 + vec_loop_size, in2 + vec_loop_size, size - vec_loop_size);

		return dist;
	}

	static size_t apply(const int8_t *in, const unsigned size)
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
				counter8 += Fdiffcnt::apply(mipp::Reg<int8_t>(in + i));

			const auto low = mipp::cvt<int8_t,int16_t>(counter8.low());
			counter32 += mipp::cvt<int16_t,int32_t>(low.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(low.high());

			const auto high = mipp::cvt<int8_t,int16_t>(counter8.high());
			counter32 += mipp::cvt<int16_t,int32_t>(high.low ());
			counter32 += mipp::cvt<int16_t,int32_t>(high.high());
		}

		auto dist = (size_t)mipp::hadd(counter32);

#else
		const auto vec_loop_size = 0;
		size_t dist = 0;
#endif

		dist += distance_seq<int8_t,Fdiffcnt>(in + vec_loop_size, size - vec_loop_size);

		return dist;
	}
};


template <typename T, class Fdiffcnt>
size_t distance(const T *in1, const T *in2, const unsigned size)
{
	return Distance<T,Fdiffcnt>::apply(in1, in2, size);
}

template <typename T, class Fdiffcnt>
size_t distance(const T *in, const unsigned size)
{
	return Distance<T,Fdiffcnt>::apply(in, size);
}

#endif // #ifdef MIPP_AVX1

}
}