#ifndef MATRIX_UTILS_HXX__
#define MATRIX_UTILS_HXX__

#include <sstream>

#include "matrix_utils.h"

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
Full_matrix<T> sparse_to_full(const Sparse_matrix& sparse)
{
	auto full = Full_matrix<T>::zero(sparse.get_n_rows(), sparse.get_n_cols());

	for (unsigned i = 0; i < sparse.get_n_rows(); i++)
		for (unsigned j = 0; j < sparse.get_cols_from_row(i).size(); j++)
			full.add_connection(i,sparse.get_cols_from_row(i)[j]);

	return full;
}

template <typename T>
Sparse_matrix full_to_sparse(const Full_matrix<T>& full)
{
	auto sparse = Sparse_matrix::zero(full.get_n_rows(), full.get_n_cols());

	for (unsigned i = 0; i < full.get_n_rows(); i++)
		for (unsigned j = 0; j < full.get_n_cols(); j++)
			if (full[i][j])
				sparse.add_connection(i,j);

	return sparse;
}

template <typename T>
Full_matrix<T> rgemm(const Full_matrix<T>& A, const Full_matrix<T>& B)
{
	if (A.get_n_cols() != B.get_n_rows())
	{
		std::stringstream message;
		message << "'A.get_n_cols()' is different to 'B.get_n_rows()' ('A.get_n_cols()' = " << A.get_n_cols()
		        << ", 'B.get_n_rows()' = " << B.get_n_rows() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto L = A.get_n_rows();
	auto M = B.get_n_rows();
	auto N = B.get_n_cols();

	Full_matrix<T> C(L, N);

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			T sum = 0;
			for (size_t m = 0; m < M; m++)
				sum += A[l][m] * B[m][n];

			C[l][n] = sum;
		}

	return C;
}

template <typename T>
Full_matrix<T> operator*(const Full_matrix<T>& A, const Full_matrix<T>& B)
{
	return rgemm(A,B);
}

template <typename T>
Full_matrix<T> rgemmt(const Full_matrix<T>& A, const Full_matrix<T>& tB)
{
	if (A.get_n_cols() != tB.get_n_cols())
	{
		std::stringstream message;
		message << "'A.get_n_cols()' is different to 'tB.get_n_cols()' ('A.get_n_cols()' = " << A.get_n_cols()
		        << ", 'tB.get_n_cols()' = " << tB.get_n_cols() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto L =  A.get_n_rows();
	auto N = tB.get_n_rows();
	auto M = tB.get_n_cols();

	Full_matrix<T> C(L, N);

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			T sum = 0;
			for (size_t m = 0; m < M; m++)
				sum += A[l][m] * tB[n][m];

			C[l][n] = sum;
		}

	return C;
}

template <typename T>
void modulo2(Full_matrix<T>& M)
{
	for (size_t r = 0; r < M.get_n_rows(); r++)
		for (size_t c = 0; c < M.get_n_cols(); c++)
			M[r][c] &= (T)1;
}

template <typename T>
bool all_zeros(const Full_matrix<T>& M)
{
	bool all_z = true;

	for (size_t r = 0; r < M.get_n_rows(); r++)
		for (size_t c = 0; c < M.get_n_cols(); c++)
			all_z &= M[r][c] == 0;

	return all_z;
}

}
}


#endif // MATRIX_UTILS_HXX__