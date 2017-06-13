#include "LDPC_matrix_handler.hpp"

using namespace aff3ct;
using namespace tools;

void LDPC_matrix_handler
::sparse_to_full(const Sparse_matrix& sparse, Full_matrix& full)
{
	full.clear();
	full.resize(sparse.get_n_rows(), mipp::vector<int8_t>(sparse.get_n_cols(), 0));

	for (unsigned i = 0; i < sparse.get_n_rows(); i++)
		for (unsigned j = 0; j < sparse.get_cols_from_row(i).size(); j++)
			full[i][sparse.get_cols_from_row(i)[j]] = 1;
}

Sparse_matrix LDPC_matrix_handler
::full_to_sparse(const Full_matrix& full)
{
	Sparse_matrix sparse(full.size(), full.front().size());

	for (unsigned i = 0; i < full.size(); i++)
		for (unsigned j = 0; j < full[i].size(); j++)
			if (full[i][j])
				sparse.add_connection(i,j);

	return sparse;
}

Sparse_matrix LDPC_matrix_handler
::transform_H_to_G(const Sparse_matrix& H, std::vector<unsigned>& info_bits_pos)
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
::transform_H_to_G(Full_matrix& mat, std::vector<unsigned>& info_bits_pos)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
		throw std::length_error("aff3ct::tools::LDPC_G::transform_H_to_G: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	std::vector<unsigned> swapped_cols;
	LDPC_matrix_handler::create_diagonal(mat, swapped_cols);
	LDPC_matrix_handler::create_identity(mat);

	for (unsigned i = 0; i < n_row; i++) // erase the just created identity in the left part of H
		mat[i].erase( mat[i].begin(), mat[i].begin() + n_row );

	// mat dimension is now n_col*n_row (above it was n_row*n_col)
	mat.resize(n_col, mipp::vector<int8_t>(n_col-n_row,0));
	for (unsigned i = n_row; i < n_col; i++) // Add identity at the end
		mat[i][i-n_row] = 1;

	// Re-organization: get G
	for (unsigned l = (unsigned)(swapped_cols.size() / 2); l > 0; l--)
		std::swap(mat[swapped_cols[l*2-2]], mat[swapped_cols[l*2-1]]);

	// return info bits positions
	info_bits_pos.resize(n_row);

	mipp::vector<unsigned> bits_pos(n_col);
	std::iota(bits_pos.begin(), bits_pos.begin() + n_col, 0);

	for (unsigned l = 1; l <= (swapped_cols.size() / 2); l++)
		std::swap(bits_pos[swapped_cols[l*2-2]], bits_pos[swapped_cols[l*2-1]]);

	std::copy(bits_pos.begin() + (n_col-n_row), bits_pos.end(), info_bits_pos.begin());
}

void LDPC_matrix_handler
::create_diagonal(Full_matrix& mat, std::vector<unsigned>& swapped_cols)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
		throw std::length_error("aff3ct::tools::LDPC_G::create_diagonal: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

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
						mipp::vector<int8_t> column_save(n_row);
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
		throw std::length_error("aff3ct::tools::LDPC_G::create_identity: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	for (unsigned i = n_row - 1 ; i > 0; i--)
		for (unsigned j = i; j > 0; j--)
			if (mat[j-1][i])
				std::transform (mat[j-1].begin(), mat[j-1].end(), mat[i].begin(), mat[j-1].begin(), std::not_equal_to<int8_t>());
}

float LDPC_matrix_handler
::compute_density(Full_matrix& mat)
{
	unsigned n_rows = mat.size();
	unsigned n_cols = mat.front().size();

	unsigned nb_ones = 0;

	for (unsigned i = 0; i < n_rows; i++)
		for (unsigned j = 0; j < n_cols; j++)
			if (mat[i][j])
				nb_ones++;

	return ((float)nb_ones/(float)(n_rows*n_cols));
}

Sparse_matrix LDPC_matrix_handler
::interleave_matrix(const Sparse_matrix& mat, std::vector<unsigned>& new_cols_pos)
{
	if (mat.get_n_cols() != new_cols_pos.size())
		throw std::length_error("aff3ct::tools::LDPC_G::interleave_matrix: matrix width \"mat.get_n_cols()\" has to be"
		                        " equal to interleaver length \"new_cols_pos.size()\".");


	Sparse_matrix itl_mat(mat.get_n_rows(), mat.get_n_cols());

	for (unsigned i = 0; i < mat.get_n_cols(); i++)
	{
		for (unsigned j = 0; j < mat.get_rows_from_col(i).size(); j++)
			itl_mat.add_connection(mat.get_rows_from_col(i)[j], new_cols_pos[i]);
	}

	return itl_mat;
}

std::vector<unsigned> LDPC_matrix_handler
::interleave_info_bits_pos(const std::vector<unsigned>& info_bits_pos, std::vector<unsigned>& new_cols_pos)
{
	if (info_bits_pos.size() > new_cols_pos.size())
		throw std::length_error("aff3ct::tools::LDPC_G::interleave_info_bits_pos: vector length \"vec.size()\" has to"
		                        " be smaller than or equal to interleaver length \"new_cols_pos.size()\".");

	std::vector<unsigned> itl_vec(info_bits_pos.size());

	for (unsigned i = 0; i < info_bits_pos.size(); i++)
	{
		itl_vec[i] = new_cols_pos[info_bits_pos[i]];
	}

	return itl_vec;
}
