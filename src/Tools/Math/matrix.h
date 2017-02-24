#ifndef MATRIX_H
#define MATRIX_H

#include "Tools/Perf/MIPP/mipp.h"

#ifndef _MSC_VER
#ifndef __forceinline
#define __forceinline inline __attribute__((always_inline))
#endif
#endif

namespace aff3ct
{
// ------------------------------------------------------------------------------------------- special function headers

// M   - INTEGER.
//      On entry,  M  specifies  the number  of rows  of the
//      matrix op( A )  and of the  matrix  C.  M  must  be
//      at least  zero.  Unchanged on exit.

// N   - INTEGER.
//      On entry,  N  specifies the number  of columns of the
//      matrix op( B ) and the number of columns of the
//      matrix C. N must be at least zero.  Unchanged on
//      exit.

// K   - INTEGER.
//      On entry,  K  specifies  the number of columns of the
//      matrix op( A ) and the number of rows of the matrix
//      op( B ). K must be at least  zero.  Unchanged on
//      exit.

// complex general matrix multiplication: C = A * B, tB is B transposed, tC is C transposed
template <typename T> 
__forceinline void cgemm(const int M, const int N, const int K,
                         const mipp::vector<T> &A, 
                         const mipp::vector<T> &tB, 
                               mipp::vector<T> &tC);

// keep only the real part in C
template <typename T> 
__forceinline void cgemm_r(const int M, const int N, const int K,
                           const mipp::vector<T> &A, 
                           const mipp::vector<T> &tB, 
                                 mipp::vector<T> &tC);

// complex transpose : B_real = tA_real and B_imag = -tA_imag, where A is of size M*N*2 and then B of size N*M*2
// (with both complex elements) : B's elements are the conjugates of A's
template <typename T>
__forceinline void complex_transpose(const int M, const int N,
                                     const mipp::vector<T> &A,
                                           mipp::vector<T> &B);
}

#include "matrix.hxx"

#endif /* MATRIX_H */
