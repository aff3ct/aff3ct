#include "LDPC_G.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B>
void LDPC_G
::sparse_to_full(const sparse_matrix& sparse, const unsigned k, const unsigned n, full_matrix<B>& full)
{
	full.resize(k, mipp::vector<B>(n, 0));

	if (k != sparse.size())
		throw std::length_error("aff3ct::tools::LDPC_G::sparse_to_full: sparse matrix high \"sparse.size()\" has"
		                        " to be equal to its given high \"k\".");

	for (unsigned i = 0; i < sparse.size(); i++)
		for (unsigned j = 0; j < sparse[i].size(); j++)
			full[i].at(sparse[i][j]) = 1; // will check if the given position does not overflow
}

template <typename B>
void LDPC_G
::full_to_sparse(const full_matrix<B>& full, sparse_matrix& sparse, unsigned& k, unsigned& n)
{
	k = (unsigned)full.size();
	n = (unsigned)full.front().size();

	sparse.resize(k);
	for (unsigned i = 0; i < full.size(); i++)
		for (unsigned j = 0; j < full[i].size(); j++)
			if (full[i][j])
				sparse[i].push_back(j);
}

template <typename B>
void LDPC_G
::create_diagonal(full_matrix<B>& mat, mipp::vector<unsigned>& swapped_cols)
{
	unsigned k = (unsigned)mat.size();
	unsigned n = (unsigned)mat.front().size();

	if (k > n)
		throw std::length_error("aff3ct::tools::LDPC_G::create_diagonal: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	unsigned i = 0;
	bool found = false;

	while (i < k)
	{
		if (mat[i][i])
		{
			for (unsigned j = i +1; j < k; j++)
 				if( mat[j][i] )
					std::transform(mat[j].begin(), mat[j].end(), mat[i].begin(), mat[j].begin(), std::not_equal_to<B>());
		}
		else
		{
			for (unsigned j = i +1; j < k; j++) // find an other row which is good
				if (mat[j][i])
				{
					std::swap(mat[i], mat[j]);
					i--;
					found = true;
					break;
				}

			if (!found) // if does not fund
				for (unsigned j = i +1; j < n; j++) // find an other column which is good
					if (mat[i][j])
					{
						swapped_cols.push_back(i);
						swapped_cols.push_back(j);

						// swap the columns
						mipp::vector<B> column_save(k);
						for (unsigned l = 0; l < k; l++) column_save[l] = (mat[l][i]);
						for (unsigned l = 0; l < k; l++) mat[l][i] = mat[l][j];
						for (unsigned l = 0; l < k; l++) mat[l][j] = column_save[l];

						i--;
						found = true;
						break;
					}

			if (!found) // if does not fund again this mean that the row is the null vector
			{
				mat.erase(mat.begin() +i);
				i--;
				k--;
			}
			found = false;
		}
		i++;
	}
}

template <typename B>
void LDPC_G
::create_identity(full_matrix<B>& mat)
{
	unsigned k = (unsigned)mat.size();
	unsigned n = (unsigned)mat.front().size();

	if (k > n)
		throw std::length_error("aff3ct::tools::LDPC_G::create_identity: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	for (unsigned i = k - 1 ; i > 0; i--)
		for (unsigned j = i; j > 0; j--)
			if (mat[j-1][i])
				std::transform (mat[j-1].begin(), mat[j-1].end(), mat[i].begin(), mat[j-1].begin(), std::not_equal_to<B>());
}

template <typename B>
void LDPC_G
::transform_H_to_G(full_matrix<B>& mat, mipp::vector<unsigned>& info_bits_pos)
{
	unsigned k = (unsigned)mat.size();
	unsigned n = (unsigned)mat.front().size();

	if (k > n)
		throw std::length_error("aff3ct::tools::LDPC_G::transform_H_to_G: matrix high \"mat.size()\" has to be smaller "
		                        " than its width \"mat.front().size()\".");

	mipp::vector<unsigned> swapped_cols;
	tools::LDPC_G::create_diagonal(mat, swapped_cols);
	tools::LDPC_G::create_identity(mat);

	for (unsigned i = 0; i < k; i++) // erase the just created identity in the left part of H
		mat[i].erase( mat[i].begin(), mat[i].begin() + k );

	// mat dimension is now n*k (above it was k*n)
	mat.resize(n, mipp::vector<B>(n-k,0));
	for (unsigned i = k; i < n; i++) // Add identity at the end
		mat[i][i-k] = 1;

	// Re-organization: get G
	for (unsigned l = (unsigned)(swapped_cols.size() / 2); l > 0; l--)
		std::swap(mat[swapped_cols[l*2-2]], mat[swapped_cols[l*2-1]]);

	// return info bits positions
	info_bits_pos.resize(k);

	mipp::vector<unsigned> bits_pos(n);
	std::iota(bits_pos.begin(), bits_pos.begin() + n, 0);

	for (unsigned l = 1; l <= (swapped_cols.size() / 2); l++)
		std::swap(bits_pos[swapped_cols[l*2-2]], bits_pos[swapped_cols[l*2-1]]);

	std::copy(bits_pos.begin() + (n-k), bits_pos.end(), info_bits_pos.begin());
}

inline void LDPC_G
::transform_H_to_G(const sparse_matrix& H, const unsigned k_H, const unsigned n_H,
                   sparse_matrix& G, unsigned& k_G, unsigned& n_G,
                   mipp::vector<unsigned>& info_bits_pos)
{
	using bit_type = int8_t;
	full_matrix<bit_type> mat;
	sparse_to_full(H, k_H, n_H, mat);

	transform_H_to_G(mat, info_bits_pos);

	full_to_sparse(mat, G, n_G, k_G);
}
}
}
