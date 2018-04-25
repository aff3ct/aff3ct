#include <functional>
#include <sstream>
#include <fstream>

#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Code/LDPC/QC/QC.hpp"
#include "Tools/general_utils.h"

#include "Tools/Exception/exception.hpp"

#include "LDPC_matrix_handler.hpp"

using namespace aff3ct::tools;

LDPC_matrix_handler::Matrix_format LDPC_matrix_handler
::get_matrix_format(std::istream& file)
{
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

void LDPC_matrix_handler
::sparse_to_full(const Sparse_matrix& sparse, Full_matrix& full)
{
	full.clear();
	full.resize(sparse.get_n_rows(), std::vector<bool>(sparse.get_n_cols(), 0));

	for (unsigned i = 0; i < sparse.get_n_rows(); i++)
		for (unsigned j = 0; j < sparse.get_cols_from_row(i).size(); j++)
			full[i][sparse.get_cols_from_row(i)[j]] = 1;
}

Sparse_matrix LDPC_matrix_handler
::full_to_sparse(const Full_matrix& full)
{
	Sparse_matrix sparse((unsigned)full.size(), (unsigned)full.front().size());

	for (unsigned i = 0; i < full.size(); i++)
		for (unsigned j = 0; j < full[i].size(); j++)
			if (full[i][j])
				sparse.add_connection(i,j);

	return sparse;
}

Sparse_matrix LDPC_matrix_handler
::transform_H_to_G(const Sparse_matrix& H, Positions_vector& info_bits_pos)
{
	LDPC_matrix_handler::Full_matrix mat;

	if (H.get_n_rows() > H.get_n_cols())
		LDPC_matrix_handler::sparse_to_full(H.transpose(), mat);
	else
		LDPC_matrix_handler::sparse_to_full(H, mat);

	LDPC_matrix_handler::transform_H_to_G(mat, info_bits_pos);

	return LDPC_matrix_handler::full_to_sparse(mat);
}

void LDPC_matrix_handler
::transform_H_to_G(Full_matrix& mat, Positions_vector& info_bits_pos)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
	{
		std::stringstream message;
		message << "'n_row' has to be smaller or equal to 'n_col' ('n_row' = " << n_row
		        << ", 'n_col' = " << n_col << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	Positions_vector swapped_cols;
	LDPC_matrix_handler::create_diagonal(mat, swapped_cols);
	LDPC_matrix_handler::create_identity(mat);

	for (unsigned i = 0; i < n_row; i++) // erase the just created identity in the left part of H
		mat[i].erase( mat[i].begin(), mat[i].begin() + n_row );

	// mat dimension is now n_col*n_row (above it was n_row*n_col)
	mat.resize(n_col, std::vector<bool>(n_col - n_row, 0));
	for (unsigned i = n_row; i < n_col; i++) // Add identity at the end
		mat[i][i-n_row] = 1;

	// Re-organization: get G
	for (unsigned l = (unsigned)(swapped_cols.size() / 2); l > 0; l--)
		std::swap(mat[swapped_cols[l*2-2]], mat[swapped_cols[l*2-1]]);

	// return info bits positions
	info_bits_pos.resize(n_col - n_row);

	Positions_vector bits_pos(n_col);
	std::iota(bits_pos.begin(), bits_pos.end(), 0);

	for (unsigned l = 1; l <= (swapped_cols.size() / 2); l++)
		std::swap(bits_pos[swapped_cols[l*2-2]], bits_pos[swapped_cols[l*2-1]]);

	std::copy(bits_pos.begin() + n_row, bits_pos.end(), info_bits_pos.begin());
}

void LDPC_matrix_handler
::create_diagonal(Full_matrix& mat, Positions_vector& swapped_cols)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
	{
		std::stringstream message;
		message << "'n_row' has to be smaller or equal to 'n_col' ('n_row' = " << n_row
		        << ", 'n_col' = " << n_col << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	unsigned i = 0;
	bool found = false;

	while (i < n_row)
	{
		if (mat[i][i])
		{
			for (unsigned j = i +1; j < n_row; j++)
 				if( mat[j][i] )
					std::transform(mat[j].begin(), mat[j].end(), mat[i].begin(), mat[j].begin(), std::not_equal_to<int8_t>());
		}
		else
		{
			for (unsigned j = i +1; j < n_row; j++) // find an other row which is good
				if (mat[j][i])
				{
					std::swap(mat[i], mat[j]);
					i--;
					found = true;
					break;
				}

			if (!found) // if does not fund
				for (unsigned j = i +1; j < n_col; j++) // find an other column which is good
					if (mat[i][j])
					{
						swapped_cols.push_back(i);
						swapped_cols.push_back(j);

						// swap the columns
						std::vector<int8_t> column_save(n_row);
						for (unsigned l = 0; l < n_row; l++) column_save[l] = (mat[l][i]);
						for (unsigned l = 0; l < n_row; l++) mat[l][i] = mat[l][j];
						for (unsigned l = 0; l < n_row; l++) mat[l][j] = column_save[l];

						i--;
						found = true;
						break;
					}

			if (!found) // if does not fund again this mean that the row is the null vector
			{
				mat.erase(mat.begin() +i);
				i--;
				n_row--;
			}
			found = false;
		}
		i++;
	}
}

void LDPC_matrix_handler
::create_identity(Full_matrix& mat)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
	{
		std::stringstream message;
		message << "'n_row' has to be smaller or equal to 'n_col' ('n_row' = " << n_row
		        << ", 'n_col' = " << n_col << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (unsigned i = n_row - 1 ; i > 0; i--)
		for (unsigned j = i; j > 0; j--)
			if (mat[j-1][i])
				std::transform (mat[j-1].begin(), mat[j-1].end(), mat[i].begin(), mat[j-1].begin(), std::not_equal_to<int8_t>());
}

float LDPC_matrix_handler
::compute_density(Full_matrix& mat)
{
	unsigned n_rows = (unsigned)mat.size();
	unsigned n_cols = (unsigned)mat.front().size();

	unsigned nb_ones = 0;

	for (unsigned i = 0; i < n_rows; i++)
		for (unsigned j = 0; j < n_cols; j++)
			if (mat[i][j])
				nb_ones++;

	return ((float)nb_ones/(float)(n_rows*n_cols));
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

LDPC_matrix_handler::QCFull_matrix LDPC_matrix_handler
::invert_H2(const Sparse_matrix& _H)
{
	Sparse_matrix H;
	if (_H.get_n_rows() > _H.get_n_cols())
		H = _H.transpose();
	else
		H = _H;

	unsigned M = H.get_n_rows();
	unsigned N = H.get_n_cols();
	unsigned K = N - M;

	QCFull_matrix MatriceCalcul;
	MatriceCalcul.clear();
	MatriceCalcul.resize(M, mipp::vector<int8_t>(2 * M, 0));

	//Copy H2 on left
	for (unsigned i = 0; i < M; i++)
		for (unsigned j = 0; j < H.get_cols_from_row(i).size(); j++)
			if (H.get_cols_from_row(i)[j] >= K)
				MatriceCalcul[i][H.get_cols_from_row(i)[j] - K] = 1;

	//Create identity on right
	for (unsigned i = 0; i < M; i++)
			MatriceCalcul[i][M + i] = 1;

	//Pivot de Gauss (Forward)
	for (unsigned indLgn = 0; indLgn < M; indLgn++)
	{
		if (MatriceCalcul[indLgn][indLgn] == 0)
		{
			unsigned indLgnSwap = 0;
			for (unsigned indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
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
		for (unsigned indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (unsigned)(2 * M / mipp::nElReg<int8_t>());
				for (unsigned i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<int8_t>(&MatriceCalcul[indLgn][i * mipp::nElReg<int8_t>()]);
					const auto rLgn2 = mipp::Reg<int8_t>(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
				}
				for (unsigned i = loop_size1 * mipp::nElReg<int8_t>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}

	//Pivot de Gauss (Backward)
	for (unsigned indLgn = M - 1; indLgn > 0; indLgn--)
	{
		//XOR des lignes
		for (unsigned indLgn2 = indLgn - 1; indLgn2 > 0; indLgn2--)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (unsigned)(2 * M / mipp::nElReg<int8_t>());
				for (unsigned i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<int8_t>(&MatriceCalcul[indLgn][i * mipp::nElReg<int8_t>()]);
					const auto rLgn2 = mipp::Reg<int8_t>(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
				}
				for (unsigned i = loop_size1 * mipp::nElReg<int8_t>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}

	QCFull_matrix invH2;
	invH2.clear();
	invH2.resize(M, mipp::vector<int8_t>(M, 0));

	for (unsigned i = 0; i < M; i++)
		for (unsigned j = 0; j < M; j++)
			invH2[i][j] = MatriceCalcul[i][M + j];

	MatriceCalcul.clear();

	return invH2;
}
