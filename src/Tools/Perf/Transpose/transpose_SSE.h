#ifdef __SSE4_1__

#ifndef TRANSPOSE_SSE_H
#define	TRANSPOSE_SSE_H

#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace aff3ct
{
namespace tools
{
void sse_trans_float (float *A, float *B, int n);
void sse_itrans_float(float *A, float *B, int n);

void uchar_transpose_sse (const __m128i *src, __m128i *dst, int n);
void uchar_itranspose_sse(const __m128i *src, __m128i *dst, int n);
}
}

#endif	/* TRANSPOSE_SSE_H */

#endif
