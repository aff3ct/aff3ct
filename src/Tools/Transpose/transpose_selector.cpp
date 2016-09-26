#include "transpose_selector.h"

#include <iostream>

#include "Tools/bash_tools.h"

#ifdef __AVX2__
#include "transpose_AVX.h"
#elif defined(__SSE4_1__)
#include "transpose_SSE.h"
#endif
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include "transpose_NEON.h"
#endif

bool char_transpose(const signed char *src, signed char *dst, int n)
{
	bool is_transposed = false;
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	is_transposed = false;
#elif defined(__AVX2__)
	if (n >= 256)
	{
		uchar_transpose_avx((__m256i*) src, (__m256i*) dst, n);
		is_transposed = true;
	}
#elif defined(__SSE4_1__)
	if (n >= 128)
	{
		uchar_transpose_sse((__m128i*) src, (__m128i*) dst, n);
		is_transposed = true;
	}
#elif (defined(__ARM_NEON__) || defined(__ARM_NEON))
	if (n >= 128)
	{
		uchar_transpose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n);
		is_transposed = true;
	}
#else
	std::cerr << bold_red("(EE) Transposition does not support this architecture ")
	          << bold_red("(supported architectures are: NEON, NEONv2, SSE4.1 and AVX2).")
	          << std::endl;
	exit(-1);
#endif

	return is_transposed;
}

bool char_itranspose(const signed char *src, signed char *dst, int n)
{
	bool is_itransposed = false;
#if defined(__MIC__) || defined(__KNCNI__) || defined(__AVX512__) || defined(__AVX512F__)
	is_itransposed = false;
#elif defined(__AVX2__)
	if (n >= 256)
	{
		uchar_itranspose_avx((__m256i*) src, (__m256i*) dst, n / 8);
		is_itransposed = true;
	}
#elif defined(__SSE4_1__)
	if (n >= 128)
	{
		uchar_itranspose_sse((__m128i*) src, (__m128i*) dst, n / 8);
		is_itransposed = true;
	}
#elif (defined(__ARM_NEON__) || defined(__ARM_NEON))
	if (n >= 128)
	{
		uchar_itranspose_neon((trans_TYPE*) src, (trans_TYPE*) dst, n / 8);
		is_itransposed = true;
	}
#else
	std::cerr << bold_red("(EE) iTransposition does not support this architecture ")
	          << bold_red("(supported architectures are: NEON, NEONv2, SSE4.1 and AVX2).")
	          << std::endl;
	exit(-1);
#endif

	return is_itransposed;
}
