#ifndef LDPC_MATRIX_HANDLER_HPP_
#define LDPC_MATRIX_HANDLER_HPP_

#include <vector>
#include <algorithm>
#include <numeric>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct LDPC_matrix_handler
{
public:
	using Full_matrix = std::vector<std::vector<bool>>;

	/*
	 * convert a binary sparse matrix to a binary full matrix
	 */
	static void sparse_to_full(const Sparse_matrix& sparse, Full_matrix& full);

	/*
	 * convert a binary full matrix to a binary sparse matrix
	 */
	static Sparse_matrix full_to_sparse(const Full_matrix& full);

	/*
	 * Reorder rows and columns to create a diagonal of binary ones on the left part of the matrix.
	 * High of the matrix must be smaller than its width.
	 * At the end, the left part of the matrix does not necessary form the identity, but includes it.
	 * swapped_cols is completed each time with couple of positions of the two swapped columns.
	 * A column might be swapped several times.
	 */
	static void create_diagonal(Full_matrix& mat, std::vector<unsigned>& swapped_cols);

	/*
	 * Reorder rows and columns to create an identity of binary ones on the left part of the matrix.
	 * High of the matrix must be smaller than its width.
	 */
	static void create_identity(Full_matrix& mat);

	/*
	 * Return the density of ones in the given matrix.
	 */
	static float compute_density(Full_matrix & mat);

	/*
	 * Compute a G matrix related to the given H matrix.
	 * Warning G is transposed !
	 * Return also the information bits positions in the returned G matrix.
	 */
	static Sparse_matrix transform_H_to_G(const Sparse_matrix& H, std::vector<unsigned>& info_bits_pos);

	/*
	 * integrate an interleaver inside the matrix to avoid this step.
	 * old_cols_pos gives the old position of the matching column.
	 * For ex. the column old_cols_pos[i] of mat will be set at position i
	 */
	static Sparse_matrix interleave_matrix(const Sparse_matrix& mat, std::vector<unsigned>& old_cols_pos);

	/*
	 * return the vector of the position of the info bits after interleaving.
	 * old_cols_pos gives the old position of the matching column.
	 * For ex. the bit position info_bits_pos[i] will be set at position j where
	 * old_cols_pos[j] == info_bits_pos[i]
	 */
	static std::vector<unsigned> interleave_info_bits_pos(const std::vector<unsigned>& info_bits_pos,
	                                                      std::vector<unsigned>& old_cols_pos);

protected :

	static void transform_H_to_G(Full_matrix& mat, std::vector<unsigned>& info_bits_pos);

};
}
}

#endif /* LDPC_MATRIX_HANDLER_HPP_ */
