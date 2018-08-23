#include <functional>
#include <sstream>
#include <fstream>

#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Code/LDPC/QC/QC.hpp"
#include "Tools/general_utils.h"
#include "Tools/Math/matrix.h"

#include "Tools/Exception/exception.hpp"

#include "LDPC_matrix_handler.hpp"

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
			S = tools::QC::read(file);

			if (pct_pattern != nullptr)
				*pct_pattern = tools::QC::read_pct_pattern(file);

			break;
		}
		case Matrix_format::ALIST:
		{
			S = tools::AList::read(file);

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
::transform_H_to_G(const Sparse_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	LDPC_matrix Hf = sparse_to_full<LDPC_matrix::value_type>(H);

	auto Gf = LDPC_matrix_handler::transform_H_to_G(Hf, info_bits_pos);

	return full_to_sparse(Gf);
}

LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::transform_H_to_G(const LDPC_matrix& H, Positions_vector& info_bits_pos)
{
	H.is_of_way_throw(Matrix::Way::HORIZONTAL);
	auto G = H;

	auto M = G.get_n_rows();
	auto N = G.get_n_cols();
	auto K = N - M;

	Positions_vector swapped_cols;
	LDPC_matrix_handler::form_diagonal(G, swapped_cols);
	LDPC_matrix_handler::form_identity(G);

	// erase the just created M*M identity in the left part of H and add the K*K identity below
	G.self_resize(N, K, Matrix::Origin::TOP_RIGHT);
	for (auto i = M; i < N; i++) // Add rising diagonal identity at the end
		G[i][i - M] = 1;

	// G is now VERTICAL

	// Re-organization: get G
	for (auto l = (swapped_cols.size() / 2); l > 0; l--)
		std::swap(G[swapped_cols[l*2-2]], G[swapped_cols[l*2-1]]);

	// return info bits positions
	info_bits_pos.resize(K);

	Positions_vector bits_pos(N);
	std::iota(bits_pos.begin(), bits_pos.end(), 0);

	for (size_t l = 1; l <= (swapped_cols.size() / 2); l++)
		std::swap(bits_pos[swapped_cols[l*2-2]], bits_pos[swapped_cols[l*2-1]]);

	std::copy(bits_pos.begin() + M, bits_pos.end(), info_bits_pos.begin());

	return G;
}

void LDPC_matrix_handler
::form_diagonal(LDPC_matrix& mat, Positions_vector& swapped_cols)
{
	mat.self_turn(Matrix::Way::HORIZONTAL);

	auto n_row = mat.get_n_rows();
	auto n_col = mat.get_n_cols();

	size_t i = 0;
	bool found = false;

	while (i < n_row)
	{
		if (mat[i][i])
		{
			for (auto j = i +1; j < n_row; j++)
 				if (mat[j][i])
					std::transform(mat[j].begin(), mat[j].end(),
					               mat[i].begin(), mat[j].begin(),
					               std::not_equal_to<LDPC_matrix::value_type>());
		}
		else
		{
			for (auto j = i +1; j < n_row; j++) // find an other row which is good
				if (mat[j][i])
				{
					std::swap(mat[i], mat[j]);
					i--;
					found = true;
					break;
				}

			if (!found) // if does not found
				for (auto j = i +1; j < n_col; j++) // find an other column which is good
					if (mat[i][j])
					{
						swapped_cols.push_back(i);
						swapped_cols.push_back(j);

						// swap the columns
						std::vector<int8_t> column_save(n_row);
						for (size_t l = 0; l < n_row; l++) column_save[l] = (mat[l][i]);
						for (size_t l = 0; l < n_row; l++) mat[l][i] = mat[l][j];
						for (size_t l = 0; l < n_row; l++) mat[l][j] = column_save[l];

						i--;
						found = true;
						break;
					}

			if (!found) // if does not found again this mean that the row is the null vector
			{
				mat.erase_row(i);
				i--;
				n_row--;
			}
			found = false;
		}
		i++;
	}
}

void LDPC_matrix_handler
::form_identity(LDPC_matrix& mat)
{
	mat.self_turn(Matrix::Way::HORIZONTAL);

	for (auto i = mat.get_n_rows() - 1 ; i > 0; i--)
		for (auto j = i; j > 0; j--)
			if (mat[j-1][i])
				std::transform (mat[j-1].begin(), mat[j-1].end(),
				                mat[i  ].begin(), mat[j-1].begin(),
				                std::not_equal_to<LDPC_matrix::value_type>());
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

LDPC_matrix_handler::LDPC_matrix LDPC_matrix_handler
::invert_H2(const Sparse_matrix& _H)
{
	using V = LDPC_matrix::value_type;

	auto H = _H.turn(Matrix::Way::HORIZONTAL);

	auto M = H.get_n_rows();
	auto N = H.get_n_cols();
	auto K = N - M;

	LDPC_matrix MatriceCalcul(M, 2 * M);

	//Copy H2 on left
	for (size_t i = 0; i < M; i++)
		for (size_t j = 0; j < H.get_cols_from_row(i).size(); j++)
			if (H.get_cols_from_row(i)[j] >= K)
				MatriceCalcul[i][H.get_cols_from_row(i)[j] - K] = 1;

	//Create identity on right
	for (size_t i = 0; i < M; i++)
			MatriceCalcul[i][M + i] = 1;

	//Pivot de Gauss (Forward)
	for (size_t indLgn = 0; indLgn < M; indLgn++)
	{
		if (MatriceCalcul[indLgn][indLgn] == 0)
		{
			size_t indLgnSwap = 0;
			for (auto indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
			{
				if (MatriceCalcul[indLgn2][indLgn] != 0)
				{
					indLgnSwap = indLgn2;
					break;
				}
			}

			if (indLgnSwap == 0)
			{
				std::stringstream message;
				message << "Matrix H2 (H = [H1 H2]) is not invertible";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			std::swap(MatriceCalcul[indLgn], MatriceCalcul[indLgnSwap]);
		}

		//XOR des lignes
		for (auto indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (size_t)(2 * M / mipp::nElReg<V>());
				for (size_t i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<V>(&MatriceCalcul[indLgn ][i * mipp::nElReg<V>()]);
					const auto rLgn2 = mipp::Reg<V>(&MatriceCalcul[indLgn2][i * mipp::nElReg<V>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<V>()]);
				}
				for (unsigned i = loop_size1 * mipp::nElReg<V>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}

	//Pivot de Gauss (Backward)
	for (auto indLgn = M - 1; indLgn > 0; indLgn--)
	{
		//XOR des lignes
		for (auto indLgn2 = indLgn - 1; indLgn2 > 0; indLgn2--)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (size_t)(2 * M / mipp::nElReg<V>());
				for (size_t i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<V>(&MatriceCalcul[indLgn][i * mipp::nElReg<V>()]);
					const auto rLgn2 = mipp::Reg<V>(&MatriceCalcul[indLgn2][i * mipp::nElReg<V>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<V>()]);
				}
				for (auto i = loop_size1 * mipp::nElReg<V>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}


	MatriceCalcul.resize(M, M, Matrix::Origin::TOP_RIGHT);

	return MatriceCalcul;
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
