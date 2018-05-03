#ifndef LDPC_MATRIX_HANDLER_HPP_
#define LDPC_MATRIX_HANDLER_HPP_

#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <mipp.h>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct LDPC_matrix_handler
{
public:
	using Full_matrix   = std::vector<std::vector<bool>>;
	using QCFull_matrix = std::vector<mipp::vector<int8_t>>;
	using Positions_vector = std::vector<uint32_t>;

	enum class Matrix_format : int8_t {ALIST, QC};

	/*
	 * read the matrix from the given file
	 */
	static Sparse_matrix read(const std::string& filename, Positions_vector* info_bits_pos = nullptr,
	                          std::vector<bool>* pct_pattern = nullptr);

	static Sparse_matrix read(std::ifstream &file, Positions_vector* info_bits_pos = nullptr,
	                          std::vector<bool>* pct_pattern = nullptr);

	/*
	 * try to guess the matrix format from the given input stream
	 */
	static Matrix_format get_matrix_format(const std::string& filename);
	static Matrix_format get_matrix_format(std::ifstream& file);

	/*
	 * get the matrix dimensions H and N from the input stream
	 * @H is the height of the matrix
	 * @N is the width of the matrix
	 */
	static void read_matrix_size(const std::string& filename, int& H, int& N);
	static void read_matrix_size(std::ifstream &file, int& H, int& N);


	/*
	 * Check if the input info bits position are in the matrix dimensions (K*N)
	 * when @throw_when_wrong is true and the check is wrong, then throw a runtim_error, else return false.
	 */
	static bool check_info_pos(const Positions_vector& info_bits_pos, int K, int N, bool throw_when_wrong = true);


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
	static void create_diagonal(Full_matrix& mat, Positions_vector& swapped_cols);

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
	static Sparse_matrix transform_H_to_G(const Sparse_matrix& H, Positions_vector& info_bits_pos);

	/*
	 * integrate an interleaver inside the matrix to avoid this step.
	 * old_cols_pos gives the old position of the matching column.
	 * For ex. the column old_cols_pos[i] of mat will be set at position i
	 */
	static Sparse_matrix interleave_matrix(const Sparse_matrix& mat, Positions_vector& old_cols_pos);

	/*
	 * return the vector of the position of the info bits after interleaving.
	 * old_cols_pos gives the old position of the matching column.
	 * For ex. the bit position info_bits_pos[i] will be set at position j where
	 * old_cols_pos[j] == info_bits_pos[i]
	 */
	static Positions_vector interleave_info_bits_pos(const Positions_vector& info_bits_pos,
	                                                      Positions_vector& old_cols_pos);

	/*
	 * inverse H2 (H = [H1 H2] with size(H2) = M x M) to allow encoding with p = H1 x inv(H2) x u
	 */
	static QCFull_matrix invert_H2(const Sparse_matrix& H);

protected :
	static void transform_H_to_G(Full_matrix& mat, Positions_vector& info_bits_pos);
};
}
}

#endif /* LDPC_MATRIX_HANDLER_HPP_ */
