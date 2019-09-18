#include <functional>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <mipp.h>

#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Code/LDPC/QC/QC.hpp"
#include "Tools/general_utils.h"
#include "Tools/Math/matrix.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Matrix/matrix_utils.h"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

LDPC_matrix_handler::Matrix_format LDPC_matrix_handler
::get_matrix_format(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::stringstream message;
		message << "'filename' couldn't be opened ('filename' = " << filename << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return get_matrix_format(file);
}

LDPC_matrix_handler::Matrix_format LDPC_matrix_handler
::get_matrix_format(std::ifstream& file)
{
	file.seekg(0);

	std::string line;
	tools::getline(file, line);

	auto values = tools::split(line);

	if (values.size() == 3)
		return Matrix_format::QC;

	if (values.size() == 2)
		return Matrix_format::ALIST;

	std::stringstream message;
	message << "The given LDPC matrix file does not represent a known matrix type (ALIST, QC).";
	throw runtime_error(__FILE__, __LINE__, __func__, message.str());
}


Sparse_matrix LDPC_matrix_handler
::read(const std::string& filename, Positions_vector* info_bits_pos, std::vector<bool>* pct_pattern)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::stringstream message;
		message << "'filename' couldn't be opened ('filename' = " << filename << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return read(file, info_bits_pos, pct_pattern);
}

Sparse_matrix LDPC_matrix_handler
::read(std::ifstream& file, Positions_vector* info_bits_pos, std::vector<bool>* pct_pattern)
{
	auto format = get_matrix_format(file);

	file.seekg(0);

	Sparse_matrix S;

	switch (format)
	{
		case Matrix_format::QC:
		{
			S = std::move(tools::QC::read(file));

			if (pct_pattern != nullptr)
				*pct_pattern = tools::QC::read_pct_pattern(file);

			break;
		}
		case Matrix_format::ALIST:
		{
			S = std::move(tools::AList::read(file));

			if (info_bits_pos != nullptr)
				try
				{
					*info_bits_pos = tools::AList::read_info_bits_pos(file);
				}
				catch (std::exception const&)
				{
					info_bits_pos->clear();
					// information bits positions are not in the matrix file
				}
			break;
		}
	}

	return S;
}

void LDPC_matrix_handler
::read_matrix_size(const std::string& filename, int& H, int& N)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::stringstream message;
		message << "'filename' couldn't be opened ('filename' = " << filename << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return read_matrix_size(file, H, N);
}

void LDPC_matrix_handler
::read_matrix_size(std::ifstream &file, int& H, int& N)
{
	auto format = get_matrix_format(file);

	file.seekg(0);

	switch (format)
	{
		case Matrix_format::QC:
		{
			tools::QC::read_matrix_size(file, H, N);
			break;
		}
		case Matrix_format::ALIST:
		{
			tools::AList::read_matrix_size(file, H, N);
			break;
		}
	}
}

bool LDPC_matrix_handler
::check_info_pos(const Positions_vector& info_bits_pos, int K, int N, bool throw_when_wrong)
{
	if (info_bits_pos.size() != (unsigned)K)
	{
		if (!throw_when_wrong)
			return false;

		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << K << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto pos : info_bits_pos)
		if (pos >= (unsigned)N)
		{
			if (!throw_when_wrong)
				return false;

			std::stringstream message;
			message << "'pos' has to be smaller than 'N' ('pos' = " << pos << ", 'N' = " << N << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	return true;
}

Sparse_matrix LDPC_matrix_handler
::transform_H_to_G_decomp_LU(const Sparse_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	LDPC_matrix Hf = sparse_to_full<LDPC_matrix::value_type>(H);

	auto Gf = LDPC_matrix_handler::transform_H_to_G_decomp_LU(Hf, info_bits_pos);

	return full_to_sparse(Gf);
}

Sparse_matrix LDPC_matrix_handler
::transform_H_to_G_identity(const Sparse_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	LDPC_matrix Hf = sparse_to_full<LDPC_matrix::value_type>(H);

	auto Gf = LDPC_matrix_handler::transform_H_to_G_identity(Hf, info_bits_pos);

	return full_to_sparse(Gf);
}

void swap_columns(LDPC_matrix_handler::LDPC_matrix& mat, size_t idx1, size_t idx2)
{
	auto n_row = mat.get_n_rows();
	std::vector<LDPC_matrix_handler::LDPC_matrix::value_type> tmp(n_row);
	for (size_t l = 0; l < n_row; l++) tmp[l]       = mat[l][idx1];
	for (size_t l = 0; l < n_row; l++) mat[l][idx1] = mat[l][idx2];
	for (size_t l = 0; l < n_row; l++) mat[l][idx2] = tmp[l];
}


LDPC_matrix_handler::LDPC_matrix LU_decomp(const LDPC_matrix_handler::LDPC_matrix& Hp)
{
	using V = LDPC_matrix_handler::LDPC_matrix::value_type;

	Hp.is_of_way_throw(Matrix::Way::HORIZONTAL);

	auto M = Hp.get_n_rows();

	LDPC_matrix_handler::LDPC_matrix Hinv = Hp.resize(M, 2 * M, Matrix::Origin::TOP_LEFT);

	// Create identity on right part
	for (size_t i = 0; i < M; i++)
			Hinv[i][M + i] = 1;

	// Gaussian elimination (Forward)
	for (size_t r = 0; r < M; r++)
	{
		if (Hinv[r][r] == 0)
		{
			size_t r_swap = 0;
			for (auto r2 = r + 1; r2 < M; r2++)
			{
				if (Hinv[r2][r] != 0)
				{
					r_swap = r2;
					break;
				}
			}

			if (r_swap == 0)
			{
				std::stringstream message;
				message << "Matrix H2 (H = [H1 H2]) is not invertible";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			std::swap(Hinv[r], Hinv[r_swap]);
		}

		// XOR on lines
		for (auto r2 = r + 1; r2 < M; r2++)
		{
			if (Hinv[r2][r] != 0)
			{
				const auto loop_size1 = (size_t)(2 * M / mipp::nElReg<V>());
				for (size_t i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<V>(&Hinv[r ][i * mipp::nElReg<V>()]);
					const auto rLgn2 = mipp::Reg<V>(&Hinv[r2][i * mipp::nElReg<V>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&Hinv[r2][i * mipp::nElReg<V>()]);
				}
				for (size_t i = loop_size1 * (size_t)mipp::nElReg<V>(); i < 2 * M; i++)
					Hinv[r2][i] = Hinv[r2][i] ^ Hinv[r][i];
			}
		}
	}

	// Gaussian elimination (Backward)
	for (auto r = M - 1; r > 0; r--)
	{
		// XOR on lines
		for (auto r2 = r; r2 > 0; r2--)
		{
			auto rm1 = r2 - 1;
			if (Hinv[rm1][r] != 0)
			{
				const auto loop_size1 = (size_t)(2 * M / mipp::nElReg<V>());
				for (size_t i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<V>(&Hinv[r  ][i * mipp::nElReg<V>()]);
					const auto rLgn2 = mipp::Reg<V>(&Hinv[rm1][i * mipp::nElReg<V>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&Hinv[rm1][i * mipp::nElReg<V>()]);
				}
				for (auto i = loop_size1 * mipp::nElReg<V>(); i < 2 * M; i++)
					Hinv[rm1][i] = Hinv[rm1][i] ^ Hinv[r][i];
			}
		}
	}


	Hinv.self_resize(M, M, Matrix::Origin::TOP_RIGHT);

	return Hinv;
}

/* // Benjamin's version
template<bool allow_rank_deficient = true>
LDPC_matrix_handler::LDPC_matrix LU_decomp2(const LDPC_matrix_handler::LDPC_matrix& Hp)
{
	using V = LDPC_matrix_handler::LDPC_matrix::value_type;

	auto mat = Hp;

	auto n_rows = mat.get_n_rows();
	auto n_cols = mat.get_n_cols();

	auto n_min = std::min(n_rows, n_cols);
	bool rank_deficient = false;
	LDPC_matrix_handler::Positions_vector null_rows;


	// add an identity to the right part
	mat.self_resize(n_rows, n_cols + n_rows, Matrix::Origin::TOP_LEFT);
	for (size_t i = 0; i < n_rows; i++) // Add falling diagonal identity at the end
		mat[i][i + n_cols] = true;

	LDPC_matrix_handler::Positions_vector pos_rows(n_rows), pos_cols(n_cols);
	std::iota(pos_rows.begin(), pos_rows.end(), 0);
	std::iota(pos_cols.begin(), pos_cols.end(), 0);

	// Gaussian elimination (Forward)
	// try to generate an identity diagonal in the left part
	for (size_t d = 0; d < n_min; d++)
	{
		// permutation step
		bool found = false;
		for (auto c = d; c < n_cols; c++)
		{
			for (size_t r = d; r < n_rows; r++)
				if (mat[r][c])
				{
					if (r != d)
					{
						// row permutation
						std::swap(pos_rows[d], pos_rows[r]);
						std::swap(mat     [d], mat     [r]);
					}

					if (c != d)
					{
						// col permutation
						std::swap(pos_cols[d], pos_cols[c]);
						swap_columns(mat, d, c);
					}

					found = true;
					break;
				}

			if (found)
				break;
		}

		if (!found)
		{
			n_min = d;
			rank_deficient = true;
			throw runtime_error(__FILE__, __LINE__, __func__, "The matrix H is rank deficient, the SPARSE method can't handle it for now.");
			break;
		}

		// elimination step
		for (auto r = d + 1; r < n_rows; r++)
			if (mat[r][d])
				std::transform(mat[d].begin() + d + 1, mat[d].end(), //ref
				               mat[r].begin() + d + 1, mat[r].begin() + d + 1,
				               std::not_equal_to<V>());
	}


	// keep the right part of the initial identity and add zeros below to find back original mat size
	auto X = mat.resize(n_min, n_rows, Matrix::Origin::TOP_RIGHT); // delete rows below n_min
	X.self_resize(n_cols, n_rows, Matrix::Origin::TOP_LEFT); // add null rows below part if n_cols != n_min


	// Gaussian elimination (Backward)
	// back substitution
	X.self_transpose(); // X is now HORIZONTAL
	for (size_t r = 0; r < X.get_n_rows(); r++)
		for (size_t d = n_min - 1; d > 0; d--)
			for (size_t c = d; c < n_min; c++)
				X[r][d - 1] = X[r][d - 1] != ((mat[d - 1][c] != (V)0) && (X[r][c] != (V)0)); // row of 'mat' * row of 'X'


	if (allow_rank_deficient && rank_deficient)
	{
		auto Hp2 = Hp;
		// find null rows of X and delete them in Hp2
		for (auto r = X.get_n_rows(); r > 0; r--)
		{
			auto r_test = r - 1;
			if (std::find(X[r_test].begin(), X[r_test].end(), (V)true) == X[r_test].end())
				Hp2.erase_row(r_test);
		}

		X.self_transpose(); // X is now VERTICAL

		// permute rows of "X" according to permuted cols of "mat"
		for (size_t i = 0; i < n_cols; i++)
			std::swap(X[i], X[pos_cols[i]]);

		// find null cols of X and delete them in Hp2
		for (auto r = X.get_n_rows(); r > 0; r--)
		{
			auto r_test = r - 1;
			if (std::find(X[r_test].begin(), X[r_test].end(), (V)true) == X[r_test].end())
				Hp2.erase_col(r_test);
		}

		return LU_decomp2<false>(Hp2);
	}
	else
	{
		X.self_transpose(); // X is now VERTICAL

		// permute rows of "X" according to permuted cols of "mat"
		for (size_t i = 0; i < n_cols; i++)
			std::swap(X[i], X[pos_cols[i]]);

		return X;
	}
}*/

LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::LU_decomposition(const Sparse_matrix& H)
{
	using V = LDPC_matrix::value_type;

	auto Ht = H.turn(Matrix::Way::HORIZONTAL);

	auto M = Ht.get_n_rows();
	Ht.self_resize(M, M, Matrix::Origin::TOP_RIGHT);

	return LU_decomp(tools::sparse_to_full<V>(Ht));
}

LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::LU_decomposition(const LDPC_matrix& H)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);

	auto M = H.get_n_rows();
	auto Hp = H.resize(M, M, Matrix::Origin::TOP_RIGHT); // parity part of H -> Horizontal M * M

	return LU_decomp(Hp);
}

// Benjamin's version
LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::transform_H_to_G_decomp_LU(const LDPC_matrix& H, Positions_vector& info_bits_pos)
{
	auto M = H.get_n_rows();
	auto N = H.get_n_cols();
	auto K = N - M;

	LDPC_matrix Gp = LU_decomposition(H); // Gp is VERTICAL -> M * M


	auto Hs = H.resize(M, K, Matrix::Origin::TOP_LEFT); // systematic part of H
	Hs.self_transpose();
	auto GH = bgemmt(Gp, Hs); // Gp * Hs -> horizontal M * K
	// GH.self_resize(GH.get_n_rows(), GH.get_n_cols() - M, Matrix::Origin::TOP_LEFT); // remove right part
	GH.self_transpose(); // now vertical K * M

	// add identity on the left part
	GH.self_resize(K, N, Matrix::Origin::TOP_RIGHT);
	for (size_t r = 0; r < K; r++)
		GH[r][r] = 1;

	// GH is now G -> K * N


	// // check that G is good
	// Hp.self_transpose(); // now vertical M*M
	// auto GHp = bgemmt(Gp, Hp); // Gp * Hp

	// // remove the identy
	// auto min_size = std::min(GHp.get_n_rows(), GHp.get_n_cols());
	// for (size_t r = 0; r < min_size; r++)
	// 	GHp[r][r] = !GHp[r][r];

	// if (!all_zeros(GHp))
	// 	throw runtime_error(__FILE__, __LINE__, __func__, "Generated Gp from H is not good (Gp * Hp != Identity).");

	info_bits_pos.resize(K);
	std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);

	return GH;
}

// Valentin's version
LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::transform_H_to_G_identity(const LDPC_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	auto G = H;

	auto M = H.get_n_rows();
	auto N = H.get_n_cols();
	auto K = N - M;

	auto swapped_cols = LDPC_matrix_handler::form_diagonal(G, Matrix::Origin::TOP_LEFT);
	LDPC_matrix_handler::form_identity(G, Matrix::Origin::TOP_LEFT);


	// erase the just created M*M identity in the left part of H and add the K*K identity below
	G.self_resize(N, K, Matrix::Origin::TOP_RIGHT);
	for (auto i = M; i < N; i++) // Add rising diagonal identity at the end
		G[i][i - M] = 1;

	// G is now VERTICAL

	// Re-organization: get G
	for (auto l = swapped_cols.size(); l > 0; l--)
		std::swap(G[swapped_cols[l-1].first], G[swapped_cols[l-1].second]);

	// return info bits positions
	info_bits_pos.resize(K);

	Positions_vector bits_pos(N);
	std::iota(bits_pos.begin(), bits_pos.end(), 0);

	for (auto& p : swapped_cols)
		std::swap(bits_pos[p.first], bits_pos[p.second]);

	std::copy(bits_pos.begin() + M, bits_pos.end(), info_bits_pos.begin());

	return G;
}


// Olivier's version = Valentin's version with identity formed on the right part but does not work, why ?
// This may be as fast as the Valentin's version but more hollow
/*LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::transform_H_to_G_identity(const LDPC_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	auto G = H;

	auto M = H.get_n_rows();
	auto N = H.get_n_cols();
	auto K = N - M;

	auto swapped_cols = LDPC_matrix_handler::form_diagonal(G, Matrix::Origin::BOTTOM_RIGHT);
	LDPC_matrix_handler::form_identity(G, Matrix::Origin::BOTTOM_RIGHT);


	G.self_resize(N, K, Matrix::Origin::BOTTOM_LEFT); // G is now VERTICAL N*K
	for (size_t i = 0; i < K; i++) // Add falling diagonal identity above
		G[i][i] = 1;

	// Re-organization: get G
	for (auto l = swapped_cols.size(); l > 0; l--)
		std::swap(G[swapped_cols[l-1].first], G[swapped_cols[l-1].second]);

	// return info bits positions
	info_bits_pos.resize(K);

	Positions_vector bits_pos(N);
	std::iota(bits_pos.begin(), bits_pos.end(), 0);

	for (auto& p : swapped_cols)
		std::swap(bits_pos[p.first], bits_pos[p.second]);

	std::copy(bits_pos.begin() + M, bits_pos.end(), info_bits_pos.begin());

	return G;
}*/

LDPC_matrix_handler::Positions_pair_vector LDPC_matrix_handler
::form_diagonal(LDPC_matrix& mat, Matrix::Origin o)
{
	mat.self_turn(Matrix::Way::HORIZONTAL);

	auto n_row = mat.get_n_rows();
	auto n_col = mat.get_n_cols();

	Positions_pair_vector swapped_cols;

	switch (o)
	{
		case Matrix::Origin::TOP_LEFT:
			for (size_t i = 0; i < n_row; i++)
			{
				bool found = mat[i][i];

				if (!found)
				{
					// try to find an other row which as a 1 in column i
					for (auto j = i +1; j < n_row; j++)
						if (mat[j][i])
						{
							std::swap(mat[i], mat[j]);
							found = true;
							break;
						}


					if (!found) // no other row after (i+1) of the same column i with a 1
					{
						for (auto j = i +1; j < n_col; j++) // find an other column which is good on row i
						{
							if (mat[i][j])
							{
								swapped_cols.push_back(std::make_pair(i,j));

								swap_columns(mat, i, j);

								found = true;
								break;
							}
						}
					}
				}

				if (found)
				{
					// there is a 1 on row i of the column i
					// then remove any 1 of the column i from the row (i+1)
					for (auto j = i +1; j < n_row; j++)
		 				if (mat[j][i])
							std::transform(mat[i].begin() + i, mat[i].end(), //ref
							               mat[j].begin() + i, mat[j].begin() + i,
							               std::not_equal_to<LDPC_matrix::value_type>());
				}
				else
				{
					// the row is the null vector then delete it
					mat.erase_row(i);
					i--;
					n_row--;
				}
			}
		break;
		case Matrix::Origin::TOP_RIGHT:
			for (size_t i = 0; i < n_row; i++)
			{
				auto ref_col = n_col - i - 1;
				bool found = mat[i][ref_col];

				if (!found)
				{
					// try to find an other row which as a 1 in column ref_col
					for (auto j = i +1; j < n_row; j++)
						if (mat[j][ref_col])
						{
							std::swap(mat[i], mat[j]);
							found = true;
							break;
						}


					if (!found) // no other row after (i+1) of the same column ref_col with a 1
					{
						for (auto j = ref_col; j > 0; j--) // find an other column which is good on row i
						{
							auto c = j -1;
							if (mat[i][c])
							{
								swapped_cols.push_back(std::make_pair(i,c));

								swap_columns(mat, i, c);

								found = true;
								break;
							}
						}
					}
				}

				if (found)
				{
					// there is a 1 on row i of the column ref_col
					// then remove any 1 of the column ref_col from the row (i+1)
					for (auto j = i +1; j < n_row; j++)
		 				if (mat[j][ref_col])
							std::transform(mat[i].begin(), mat[i].begin() + ref_col + 1, //ref
							               mat[j].begin(), mat[j].begin(),
							               std::not_equal_to<LDPC_matrix::value_type>());
				}
				else
				{
					// the row is the null vector then delete it
					mat.erase_row(i);
					i--;
					n_row--;
				}
			}
		break;
		case Matrix::Origin::BOTTOM_LEFT:
			for (auto i = n_row; i > 0; i--)
			{
				auto ref_row = i - 1;
				auto ref_col = n_row - ref_row - 1;
				bool found = mat[ref_row][ref_col];

				if (!found)
				{
					// try to find an other row which as a 1 in column ref_col
					for (auto j = ref_row; j > 0; j--)
					{
						auto tested_row = j - 1;
						if (mat[tested_row][ref_col])
						{
							std::swap(mat[ref_row], mat[tested_row]);
							found = true;
							break;
						}
					}


					if (!found) // no other row before (ref_row-1) of the same column ref_col with a 1
					{
						for (auto j = ref_col +1; j < n_col; j++) // find an other column which is good on row ref_row
						{
							if (mat[ref_row][j])
							{
								swapped_cols.push_back(std::make_pair(ref_col,j));

								swap_columns(mat, ref_col, j);

								found = true;
								break;
							}
						}
					}
				}

				if (found)
				{
					// there is a 1 on row ref_row of the column ref_col
					// then remove any 1 of the column ref_col from the row (ref_row-1)
					for (auto j = ref_row; j > 0; j--)
					{
						auto tested_row = j - 1;
		 				if (mat[tested_row][ref_col])
							std::transform(mat[ref_row   ].begin() + ref_col, mat[ref_row   ].end(), //ref
							               mat[tested_row].begin() + ref_col, mat[tested_row].begin() + ref_col,
							               std::not_equal_to<LDPC_matrix::value_type>());
					}
				}
				else
				{
					// the row is the null vector then delete it
					mat.erase_row(ref_row);
					n_row--;
				}
			}
		break;
		case Matrix::Origin::BOTTOM_RIGHT:
			for (auto i = n_row; i > 0; i--)
			{
				auto ref_row = i - 1;
				auto ref_col = n_col - n_row + ref_row;
				bool found = mat[ref_row][ref_col];

				if (!found)
				{
					// try to find an other row which as a 1 in column ref_col
					for (auto j = ref_row; j > 0; j--)
					{
						auto tested_row = j - 1;
						if (mat[tested_row][ref_col])
						{
							std::swap(mat[ref_row], mat[tested_row]);
							found = true;
							break;
						}
					}


					if (!found) // no other row before (ref_row-1) of the same column ref_col with a 1
					{
						for (auto j = ref_col; j > 0; j--) // find an other column which is good on row ref_row
						{
							auto tested_col = j - 1;
							if (mat[ref_row][tested_col])
							{
								swapped_cols.push_back(std::make_pair(ref_col,tested_col));

								swap_columns(mat, ref_col, tested_col);

								found = true;
								break;
							}
						}
					}
				}

				if (found)
				{
					// there is a 1 on row ref_row of the column ref_col
					// then remove any 1 of the column ref_col from the row (ref_row-1)
					for (auto j = ref_row; j > 0; j--)
					{
						auto tested_row = j - 1;
		 				if (mat[tested_row][ref_col])
							std::transform(mat[ref_row   ].begin() + ref_col, mat[ref_row   ].end(), //ref
							               mat[tested_row].begin() + ref_col, mat[tested_row].begin() + ref_col,
							               std::not_equal_to<LDPC_matrix::value_type>());
					}
				}
				else
				{
					// the row is the null vector then delete it
					mat.erase_row(ref_row);
					n_row--;
				}
			}
		break;
	}

	return swapped_cols;
}

void LDPC_matrix_handler
::form_identity(LDPC_matrix& mat, Matrix::Origin o)
{
	mat.self_turn(Matrix::Way::HORIZONTAL);

	auto n_row = mat.get_n_rows();
	auto n_col = mat.get_n_cols();
	auto diff  = n_col - n_row;

	switch (o)
	{
		case Matrix::Origin::TOP_LEFT:
			for (auto c = n_row - 1; c > 0; c--)
			{
				auto ref_row = c;
				for (auto r = c; r > 0; r--)
					if (mat[r - 1][c])
						std::transform (mat[ref_row].begin() + c, mat[ref_row].end(), // ref
						                mat[r - 1  ].begin() + c, mat[r - 1  ].begin() + c,
						                std::not_equal_to<LDPC_matrix::value_type>());
			}
		break;
		case Matrix::Origin::TOP_RIGHT:
			for (auto c = diff; c < (n_col - 1); c++)
			{
				auto ref_row = n_row - c + diff - 1;
				for (auto r = ref_row; r > 0; r--)
					if (mat[r - 1][c])
						std::transform (mat[ref_row].begin(), mat[ref_row].begin() + c + 1, // ref
						                mat[r - 1  ].begin(), mat[r - 1  ].begin(),
						                std::not_equal_to<LDPC_matrix::value_type>());
			}
		break;
		case Matrix::Origin::BOTTOM_LEFT:
			for (auto c = n_row - 1; c > 0; c--)
			{
				auto ref_row = n_row - c - 1;
				for (auto r = ref_row + 1; r < n_row; r++)
					if (mat[r][c])
						std::transform (mat[ref_row].begin() + c, mat[ref_row].end(), // ref
						                mat[r      ].begin() + c, mat[r      ].begin() + c,
						                std::not_equal_to<LDPC_matrix::value_type>());
			}
		break;
		case Matrix::Origin::BOTTOM_RIGHT:
			for (auto c = diff; c < (n_col - 1); c++)
			{
				auto ref_row = c - diff;
				for (auto r = ref_row + 1; r < n_row; r++)
					if (mat[r][c])
						std::transform (mat[ref_row].begin(), mat[ref_row].begin() + c + 1, // ref
						                mat[r      ].begin(), mat[r      ].begin(),
						                std::not_equal_to<LDPC_matrix::value_type>());
			}
		break;
	}
}

Sparse_matrix LDPC_matrix_handler
::interleave_matrix(const Sparse_matrix& mat, Positions_vector& old_cols_pos)
{
	if (mat.get_n_cols() != old_cols_pos.size())
	{
		std::stringstream message;
		message << "'mat.get_n_cols()' has to be equal to interleaver length 'old_cols_pos.size()' "
		        << "('mat.get_n_cols()' = " << mat.get_n_cols()
		        << ", 'old_cols_pos.size()' = " << old_cols_pos.size() << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	Sparse_matrix itl_mat(mat.get_n_rows(), mat.get_n_cols());

	for (unsigned i = 0; i < mat.get_n_cols(); i++)
	{
		for (unsigned j = 0; j < mat.get_rows_from_col(old_cols_pos[i]).size(); j++)
			itl_mat.add_connection(mat.get_rows_from_col(old_cols_pos[i])[j], i);
	}

	return itl_mat;
}

LDPC_matrix_handler::Positions_vector LDPC_matrix_handler
::interleave_info_bits_pos(const Positions_vector& info_bits_pos, Positions_vector& old_cols_pos)
{
	if (info_bits_pos.size() > old_cols_pos.size())
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be smaller than or equal to interleaver length 'old_cols_pos.size()' "
		        << " ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'old_cols_pos.size()' = " << old_cols_pos.size() << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	Positions_vector itl_vec(info_bits_pos.size());
	unsigned cnt = 0;

	for (unsigned i = 0; i < info_bits_pos.size(); i++)
	{
		auto it = std::find(old_cols_pos.begin(), old_cols_pos.end(), info_bits_pos[i]);

		if (it != old_cols_pos.end())
		{
			itl_vec[i] = (unsigned)std::distance(old_cols_pos.begin(), it);
			cnt++;
		}
	}

	if (cnt != itl_vec.size())
	{
		std::stringstream message;
		message << "'cnt' has to be equal to 'itl_vec.size()' ('cnt' = " << cnt
		        << ", 'itl_vec.size()' = " << itl_vec.size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return itl_vec;
}

bool LDPC_matrix_handler
::check_GH(const Sparse_matrix& H, const Sparse_matrix& G)
{
	Sparse_matrix GH;

	switch (H.get_way())
	{
		case Matrix::Way::HORIZONTAL:
			switch (G.get_way())
			{
				case Matrix::Way::HORIZONTAL:
					GH = bgemmt(H, G);
				break;
				case Matrix::Way::VERTICAL:
					GH = bgemm(H, G);
				break;
			}
		break;
		case Matrix::Way::VERTICAL:
			switch (G.get_way())
			{
				case Matrix::Way::HORIZONTAL:
					GH = bgemm(G, H);
				break;
				case Matrix::Way::VERTICAL:
					throw runtime_error(__FILE__, __LINE__, __func__, "G and H can't be both in VERTICAL way.");
				break;
			}
		break;
	}

	return all_zeros(GH);
}

bool LDPC_matrix_handler
::check_GH(const LDPC_matrix& H, const LDPC_matrix& G)
{
	LDPC_matrix GH;

	switch (H.get_way())
	{
		case Matrix::Way::HORIZONTAL:
			switch (G.get_way())
			{
				case Matrix::Way::HORIZONTAL:
					GH = bgemmt(H, G);
				break;
				case Matrix::Way::VERTICAL:
					GH = bgemm(H, G);
				break;
			}
		break;
		case Matrix::Way::VERTICAL:
			switch (G.get_way())
			{
				case Matrix::Way::HORIZONTAL:
					GH = bgemm(G, H);
				break;
				case Matrix::Way::VERTICAL:
					throw runtime_error(__FILE__, __LINE__, __func__, "G and H can't be both in VERTICAL way.");
				break;
			}
		break;
	}

	return all_zeros(GH);
}
