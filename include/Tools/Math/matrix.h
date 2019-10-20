/*!
 * \file
 * \brief Functions for matrices.
 */
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

namespace aff3ct
{
namespace tools
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

// real general matrix multiplication: C = A * B, tB is B transposed, tC is C transposed
template <typename T, class AT = std::allocator<T>>
inline void rgemm(const int M, const int N, const int K,
                  const std::vector<T,AT> &A,
                  const std::vector<T,AT> &tB,
                        std::vector<T,AT> &tC);

template <typename T>
inline void rgemm(const int M, const int N, const int K,
                  const T *A,
                  const T *tB,
                        T *tC);

// complex general matrix multiplication: C = A * B, tB is B transposed, tC is C transposed
template <typename T, class AT = std::allocator<T>>
inline void cgemm(const int M, const int N, const int K,
                  const std::vector<T,AT> &A,
                  const std::vector<T,AT> &tB,
                        std::vector<T,AT> &tC);

template <typename T>
inline void cgemm(const int M, const int N, const int K,
                  const T *A,
                  const T *tB,
                        T *tC);

// keep only the real part in C
template <typename T, class AT = std::allocator<T>>
inline void cgemm_r(const int M, const int N, const int K,
                    const std::vector<T,AT> &A,
                    const std::vector<T,AT> &tB,
                          std::vector<T,AT> &tC);

template <typename T>
inline void cgemm_r(const int M, const int N, const int K,
                    const T *A,
                    const T *tB,
                          T *tC);

// real transpose : B = tA, where A is of size M*N and then B of size N*M
template <typename T, class AT = std::allocator<T>>
inline void real_transpose(const int M, const int N,
                           const std::vector<T,AT> &A,
                                 std::vector<T,AT> &B);

template <typename T>
inline void real_transpose(const int M, const int N,
                          const T *A,
                                T *B);

// complex transpose : B_real = tA_real and B_imag = -tA_imag, where A is of size M*N*2 and then B of size N*M*2
// (with both complex elements) : B's elements are the conjugates of A's
template <typename T, class AT = std::allocator<T>>
inline void complex_transpose(const int M, const int N,
                              const std::vector<T,AT> &A,
                                    std::vector<T,AT> &B);

template <typename T>
inline void complex_transpose(const int M, const int N,
                              const T *A,
                                    T *B);
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Math/matrix.hxx"
#endif

#endif /* MATRIX_H */
