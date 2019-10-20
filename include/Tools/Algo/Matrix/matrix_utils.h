/*!
 * \file
 * \brief Functions for matrices.
 */
#ifndef MATRIX_UTILS_H__
#define MATRIX_UTILS_H__

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp"

namespace aff3ct
{
namespace tools
{
/*
 * \brief convert a binary sparse matrix to a binary full matrix
 */
template <typename T>
Full_matrix<T> sparse_to_full(const Sparse_matrix&);

/*
 * \brief convert a binary full matrix to a binary sparse matrix
 */
template <typename T>
Sparse_matrix full_to_sparse(const Full_matrix<T>&);

/*
 * \brief binary general matrix multiplication: C = A * B
 * \param A must be of size L * M
 * \param B must be of size M * N
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> bgemm(const Full_matrix<T>& A, const Full_matrix<T>& B);
Sparse_matrix  bgemm(const Sparse_matrix&  A, const Sparse_matrix&  B);


/*
 * \brief binary general matrix multiplication: C = A * B
 * \param A must be of size L * M
 * \param B must be of size M * N
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> operator*(const Full_matrix<T>& A, const Full_matrix<T>& B);
Sparse_matrix  operator*(const Sparse_matrix&  A, const Sparse_matrix&  B);

/*
 * \brief binary general matrix multiplication with B transposed: tC = A * tB, tB is B transposed, tC is C transposed
 *      Operations are optimized with MIPP
 * \param A must be of size L * M
 * \param tB must be of size N * M
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> bgemmt(const Full_matrix<T>& A, const Full_matrix<T>& tB);

/*
 * \brief binary general matrix multiplication with B transposed: tC = A * tB, tB is B transposed, tC is C transposed
 * \param A must be of size L * M
 * \param tB must be of size N * M
 * \return C of size L * N
 */
Sparse_matrix bgemmt(const Sparse_matrix& A, const Sparse_matrix& tB);

/*
 * \brief check if all elements of the matrix are zero
 */
template <typename T>
bool all_zeros(const Full_matrix<T>&);
bool all_zeros(const Sparse_matrix& );

}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Matrix/matrix_utils.hxx"
#endif

#endif // MATRIX_UTILS_H__