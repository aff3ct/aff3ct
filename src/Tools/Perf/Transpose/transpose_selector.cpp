#include <limits>
#include <cstdint>

#include "Tools/Exception/exception.hpp"
#if defined(__i386__) || defined(__x86_64__)
#include "Tools/Perf/Transpose/transpose_AVX.h"
#include "Tools/Perf/Transpose/transpose_SSE.h"
#endif
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "Tools/Perf/Transpose/transpose_NEON.h"
#endif
#include "Tools/Perf/Transpose/transpose_selector.h"

bool aff3ct::tools::char_transpose(const signed char *src, signed char *dst, int n)
{
#if defined(__AVX2__)
	do {
		const int min_n = 256;
		if (n < min_n)
			break;
		if (((uintptr_t)src) % (min_n / 8))
			break;
		if (((uintptr_t)dst) % (min_n / 8))
			break;
		uchar_transpose_avx((__m256i*) src, (__m256i*) dst, n);
		return true;
	} while (0);
#endif
#if defined(__SSE4_1__)
	do {
		const int min_n = 128;
		if (n < min_n)
			break;
		if (((uintptr_t)src) % (min_n / 8))
			break;
		if (((uintptr_t)dst) % (min_n / 8))
			break;
		uchar_transpose_sse((__m128i*) src, (__m128i*) dst, n);
		return true;
	} while (0);
#endif
#if (defined(__ARM_NEON__) || defined(__ARM_NEON))
	do {
		const int min_n = 128;
		if (n < min_n)
			break;
		uchar_transpose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n);
		return true;
	} while (0);
#endif
	return false;
}

bool aff3ct::tools::char_itranspose(const signed char *src, signed char *dst, int n)
{
#if defined(__AVX2__)
	do {
		const int min_n = 256;
		if (n < min_n)
			break;
		if (((uintptr_t)src) % (min_n / 8))
			break;
		if (((uintptr_t)dst) % (min_n / 8))
			break;
		uchar_itranspose_avx((__m256i*) src, (__m256i*) dst, n / 8);
		return true;
	} while (0);
#endif
#if defined(__SSE4_1__)
	do {
		const int min_n = 128;
		if (n < min_n)
			break;
		if (((uintptr_t)src) % (min_n / 8))
			break;
		if (((uintptr_t)dst) % (min_n / 8))
			break;
		uchar_itranspose_sse((__m128i*) src, (__m128i*) dst, n / 8);
		return true;
	} while (0);
#endif
#if (defined(__ARM_NEON__) || defined(__ARM_NEON))
	do {
		const int min_n = 128;
		if (n < min_n)
			break;
		uchar_itranspose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n / 8);
		return true;
	} while (0);
#endif
	return false;
}
