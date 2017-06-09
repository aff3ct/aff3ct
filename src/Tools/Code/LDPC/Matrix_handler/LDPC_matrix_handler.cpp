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
::transform_H_to_G(const Sparse_matrix& H, mipp::vector<unsigned>& info_bits_pos)
{
	LDPC_matrix_handler::Full_matrix mat;
	LDPC_matrix_handler::sparse_to_full(H, mat);

	LDPC_matrix_handler::transform_H_to_G(mat, info_bits_pos);

	return LDPC_matrix_handler::full_to_sparse(mat);
}

void LDPC_matrix_handler
::transform_H_to_G(Full_matrix& mat, mipp::vector<unsigned>& info_bits_pos)
{
	unsigned n_row = (unsigned)mat.size();
	unsigned n_col = (unsigned)mat.front().size();

	if (n_row > n_col)
		throw std::length_error("aff3ct::tools::LDPC_G::transform_H_to_G: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	mipp::vector<unsigned> swapped_cols;
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
::create_diagonal(Full_matrix& mat, mipp::vector<unsigned>& swapped_cols)
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
