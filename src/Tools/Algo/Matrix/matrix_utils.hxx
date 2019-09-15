#include <sstream>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Matrix/matrix_utils.h"

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
Full_matrix<T> bgemm(const Full_matrix<T>& A, const Full_matrix<T>& B)
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

	Full_matrix<T> C((unsigned)L, (unsigned)N);

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			T res = 0;
			for (size_t m = 0; m < M; m++)
				res = res != ((A[l][m] != (T)0) && (B[m][n] != (T)0));

			C[l][n] = res;
		}

	return C;
}

template <typename T>
inline Full_matrix<T> operator*(const Full_matrix<T>& A, const Full_matrix<T>& B)
{
	return bgemm(A,B);
}

template <typename T>
Full_matrix<T> bgemmt(const Full_matrix<T>& A, const Full_matrix<T>& tB)
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

	Full_matrix<T> C((unsigned)L, (unsigned)N);

	const auto M_loop_size = (M / (size_t)mipp::nElReg<T>()) * (size_t)mipp::nElReg<T>();

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			mipp::Msk<mipp::N<T>()> m_res = false;
			const mipp::Reg<T> r_zero = (T)0;
			const mipp::Reg<T> r_one  = (T)1;
			for (size_t m = 0; m < M_loop_size; m += mipp::nElReg<T>())
			{
				const auto a_in = mipp::Reg<T>(& A[l][m]);
				const auto b_in = mipp::Reg<T>(&tB[n][m]);

				m_res ^= ((a_in != r_zero) & (b_in != r_zero));
			}

			auto r_res = mipp::blend(r_one, r_zero, m_res);

			T res = mipp::hadd(r_res) & (T)1; // modulo 2
			for (size_t m = M_loop_size; m < M; m++)
				res = res != ((A[l][m] != (T)0) && (tB[n][m] != (T)0));

			C[l][n] = res;
		}

	return C;
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
