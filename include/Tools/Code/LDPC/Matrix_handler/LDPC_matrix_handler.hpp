/*!
 * \file
 * \brief Struct tools::LDPC_matrix_handler.
 */
#ifndef LDPC_MATRIX_HANDLER_HPP_
#define LDPC_MATRIX_HANDLER_HPP_

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <utility>
#include <vector>
#include <string>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp"

namespace aff3ct
{
namespace tools
{
struct LDPC_matrix_handler
{
public:
	using LDPC_matrix           = Full_matrix<int8_t>;
	using Positions_vector      = std::vector<uint32_t>;
	using Positions_pair_vector = std::vector<std::pair<size_t,size_t>>;

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
	 * \brief Reorder rows and columns to create a diagonal of binary ones from the given origin of the matrix.
	 * Matrix is turned in Horizontal way
	 * \return swapped columns positions pairs. Warning, a column might be swapped several times.
	 */
	static Positions_pair_vector form_diagonal(LDPC_matrix& mat, Matrix::Origin o = Matrix::Origin::TOP_LEFT);

	/*
	 * Reorder rows and columns to create an identity of binary ones on the left part of the matrix.
	 * This function need you call first form_diagonal().
	 */
	static void form_identity(LDPC_matrix& mat, Matrix::Origin o = Matrix::Origin::TOP_LEFT);

	/*
	 * \brief Compute a G matrix related to the given H matrix. This method favors a hallowed generator matrix build.
	 *        It uses the LU decomposition. Warning do not work yet with irregular matrices.
	 * \return G horizontal with a guarantee to have the identity on the left part.
	 * \param info_bits_pos is filled with the positions (that are 0 to K-1) of the information bits.
	 * \param H (in Horizontal way) is the parity matrix from which G is built.
	 */
	static Sparse_matrix transform_H_to_G_decomp_LU(const Sparse_matrix& H, Positions_vector& info_bits_pos);
	static LDPC_matrix   transform_H_to_G_decomp_LU(const LDPC_matrix&   H, Positions_vector& info_bits_pos);

	/*
	 * \brief Compute a G matrix related to the given H matrix. This method builds a matrix by creating an identity on
	 *        the left part of H then taking the parity part to create G.
	 * \return G vertical with not necessary an identity.
	 * \param info_bits_pos is filled with the positions (between 0 to N-1) of the information bits in G.
	 * \param H (in Horizontal way) is the parity matrix from which G is built.
	 */
	static Sparse_matrix transform_H_to_G_identity(const Sparse_matrix& H, Positions_vector& info_bits_pos);
	static LDPC_matrix   transform_H_to_G_identity(const LDPC_matrix&   H, Positions_vector& info_bits_pos);

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
	static LDPC_matrix LU_decomposition(const Sparse_matrix& H);
	static LDPC_matrix LU_decomposition(const LDPC_matrix&   H);


	/*
	 * \brief Compute a G.H to check if result is a null vector
	 *        H and G can be permuted except both vertical, the function handle their order
	 *        Best performance check is when do not have the same way
	 * \return true if G.H == 0
	 */
	static bool check_GH(const Sparse_matrix& H, const Sparse_matrix& G);


	/*
	 * \brief Compute a G.H to check if result is a null vector
	 *        H and G can be permuted except both vertical, the function handle their order
	 *        Best performance check is when both are horizontal
	 * \return true if G.H == 0
	 */
	static bool check_GH(const LDPC_matrix& H, const LDPC_matrix& G);
};
}
}

#endif /* LDPC_MATRIX_HANDLER_HPP_ */
