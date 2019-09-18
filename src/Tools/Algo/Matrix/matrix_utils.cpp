#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Matrix/matrix_utils.h"

using namespace aff3ct::tools;

Sparse_matrix aff3ct::tools::bgemm(const Sparse_matrix& A, const Sparse_matrix& B)
{
	if (A.get_n_cols() != B.get_n_rows())
	{
		std::stringstream message;
		message << "'A.get_n_cols()' is different to 'B.get_n_rows()' ('A.get_n_cols()' = " << A.get_n_cols()
		        << ", 'B.get_n_rows()' = " << B.get_n_rows() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto L = A.get_n_rows();
	auto N = B.get_n_cols();

	Sparse_matrix C(L, N);

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			size_t sum = 0; // number of common bits
			for (auto& c : A.get_cols_from_row(l))
				sum += B.at(c,n);

			if (sum & (size_t)1) // modulo 2
				C.add_connection(l, n);
		}

	return C;
}

Sparse_matrix aff3ct::tools::operator*(const Sparse_matrix& A, const Sparse_matrix& B)
{
	return bgemm(A,B);
}

Sparse_matrix aff3ct::tools::bgemmt(const Sparse_matrix& A, const Sparse_matrix& tB)
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

	Sparse_matrix C(L, N);

	for (size_t l = 0; l < L; l++)
		for (size_t n = 0; n < N; n++)
		{
			size_t sum = 0; // number of common bits
			for (auto& c : A.get_cols_from_row(l))
				sum += tB.at(n,c);

			if (sum & (size_t)1) // modulo 2
				C.add_connection(l, n);
		}

	return C;
}

bool aff3ct::tools::all_zeros(const Sparse_matrix& M)
{
	return M.get_n_connections() == 0;
}
