#ifdef __AVX2__

#ifndef TRANSPOSE_AVX_H
#define	TRANSPOSE_AVX_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace aff3ct
{
namespace tools
{
void avx_trans_float (float *A, float *B, int n);
void avx_itrans_float(float *A, float *B, int n);

void uchar_transpose_avx(const __m256i *src, __m256i *dst, int n);
void uchar_itranspose_avx(const __m256i *src, __m256i *dst, int n);
}
}

#endif	/* TRANSPOSE_AVX_H */

#endif

