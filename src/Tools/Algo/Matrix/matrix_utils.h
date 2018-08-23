#ifndef MATRIX_UTILS_H__
#define MATRIX_UTILS_H__

#include "Sparse_matrix/Sparse_matrix.hpp"
#include "Full_matrix/Full_matrix.hpp"

namespace aff3ct
{
namespace tools
{
/*
 * convert a binary sparse matrix to a binary full matrix
 */
template <typename T>
Full_matrix<T> sparse_to_full(const Sparse_matrix&);

/*
 * convert a binary full matrix to a binary sparse matrix
 */
template <typename T>
Sparse_matrix full_to_sparse(const Full_matrix<T>&);

/*
 * real general matrix multiplication: C = A * B
 * \param A must be of size L * M
 * \param B must be of size M * N
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> rgemm(const Full_matrix<T>& A, const Full_matrix<T>& B);

/*
 * real general matrix multiplication: C = A * B
 * \param A must be of size L * M
 * \param B must be of size M * N
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> operator*(const Full_matrix<T>& A, const Full_matrix<T>& B);

/*
 * real general matrix multiplication with B transposed: tC = A * tB, tB is B transposed, tC is C transposed
 * \param A must be of size L * M
 * \param tB must be of size N * M
 * \return C of size L * N
 */
template <typename T>
Full_matrix<T> rgemmt(const Full_matrix<T>& A, const Full_matrix<T>& tB);

/*
 * \brief apply a modulo 2 on each element of the matrix
 */
template <typename T>
void modulo2(Full_matrix<T>&);

/*
 * \brief check if all elements of the matrix are zero
 */
template <typename T>
bool all_zeros(const Full_matrix<T>&);



}
}

#include "matrix_utils.hxx"

#endif // MATRIX_UTILS_H__