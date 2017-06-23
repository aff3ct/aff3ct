#if defined(__ARM_NEON__) || defined(__ARM_NEON)

#ifndef TRANSPOSE_NEON_H
#define TRANSPOSE_NEON_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <arm_neon.h>

namespace aff3ct
{
namespace tools
{
#define trans_TYPE  uint8x16_t

extern void uchar_transpose_neon(const trans_TYPE *src, trans_TYPE *dst, int n);
extern void uchar_itranspose_neon(const trans_TYPE *src, trans_TYPE *dst, int n);
}
}

#endif /* TRANSPOSE_NEON_H */

#endif
