#include <limits>
#include <cstdint>

#include "Tools/Exception/exception.hpp"
#ifdef __AVX2__
#include "Tools/Perf/Transpose/transpose_AVX.h"
#elif defined(__SSE4_1__)
#include "Tools/Perf/Transpose/transpose_SSE.h"
#endif
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "Tools/Perf/Transpose/transpose_NEON.h"
#endif
#include "Tools/Perf/Transpose/transpose_selector.h"

bool aff3ct::tools::char_transpose(const signed char *src, signed char *dst, int n)
{
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	int min_n = std::numeric_limits<int>::max();
#elif defined(__AVX2__)
	int min_n = 256;
#elif defined(__SSE4_1__) || (defined(__ARM_NEON__) || defined(__ARM_NEON))
	int min_n = 128;
#else
	int min_n = std::numeric_limits<int>::max();
#endif

	if (n >= min_n)
	{
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
		return false;
#elif defined(__AVX2__)
		if (((uintptr_t)src) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'src' is unaligned memory.");
		if (((uintptr_t)dst) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'dst' is unaligned memory.");
		uchar_transpose_avx((__m256i*) src, (__m256i*) dst, n);
		return true;
#elif defined(__SSE4_1__)
		if (((uintptr_t)src) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'src' is unaligned memory.");
		if (((uintptr_t)dst) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'dst' is unaligned memory.");
		uchar_transpose_sse((__m128i*) src, (__m128i*) dst, n);
		return true;
#elif (defined(__ARM_NEON__) || defined(__ARM_NEON))
		uchar_transpose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n);
		return true;
#endif
	}

	return false;
}

bool aff3ct::tools::char_itranspose(const signed char *src, signed char *dst, int n)
{
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	int min_n = std::numeric_limits<int>::max();
#elif defined(__AVX2__)
	int min_n = 256;
#elif defined(__SSE4_1__) || (defined(__ARM_NEON__) || defined(__ARM_NEON))
	int min_n = 128;
#else
	int min_n = std::numeric_limits<int>::max();
#endif

	if (n >= min_n)
	{
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
		return false;
#elif defined(__AVX2__)
		if (((uintptr_t)src) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'src' is unaligned memory.");
		if (((uintptr_t)dst) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'dst' is unaligned memory.");
		uchar_itranspose_avx((__m256i*) src, (__m256i*) dst, n / 8);
		return true;
#elif defined(__SSE4_1__)
		if (((uintptr_t)src) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'src' is unaligned memory.");
		if (((uintptr_t)dst) % (min_n / 8))
			throw runtime_error(__FILE__, __LINE__, __func__, "'dst' is unaligned memory.");
		uchar_itranspose_sse((__m128i*) src, (__m128i*) dst, n / 8);
		return true;
#elif (defined(__ARM_NEON__) || defined(__ARM_NEON))
		uchar_itranspose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n / 8);
		return true;
#endif
	}

	return false;
}
