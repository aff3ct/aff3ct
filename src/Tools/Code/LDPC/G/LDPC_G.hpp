#ifndef LDPC_G_HPP_
#define LDPC_G_HPP_

#include <vector>
#include <algorithm>
#include <numeric>
#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
namespace tools
{
struct LDPC_G
{
public:

	using sparse_matrix = std::vector<std::vector<unsigned>>;
	template<typename B>
	using full_matrix   = std::vector<mipp::vector<B>>;

	/*
	 * convert a binary sparse matrix to a binary full matrix
	 */
	template <typename B>
	static inline void sparse_to_full(const sparse_matrix& sparse, const unsigned k, const unsigned n, full_matrix<B>& full_H);

	/*
	 * convert a binary full matrix to a binary sparse matrix
	 */
	template <typename B>
	static inline void full_to_sparse(const full_matrix<B>& full, sparse_matrix& sparse, unsigned& k, unsigned& n);

	/*
	 * Reorder rows and columns to create a diagonal of binary ones on the left part of the matrix.
	 * High of the matrix must be smaller than its width.
	 * At the end, the left part of the matrix does not necessary form the identity, but includes it.
	 */
	template <typename B>
	static inline void create_diagonal(full_matrix<B>& mat, mipp::vector<unsigned>& swapped_cols);

	/*
	 * Reorder rows and columns to create an identity of binary ones on the left part of the matrix.
	 * High of the matrix must be smaller than its width.
	 */
	template <typename B>
	static inline void create_identity(full_matrix<B>& mat);

	static inline void transform_H_to_G(const sparse_matrix& H, const unsigned k_H, const unsigned n_H,
	                             sparse_matrix& G, unsigned& k_G, unsigned& n_G,
	                             mipp::vector<unsigned>& info_bits_pos);


protected :

	template <typename B>
	static inline void transform_H_to_G(full_matrix<B>& mat, mipp::vector<unsigned>& info_bits_pos);

};
}
}

#include "LDPC_G.hxx"

#endif /*G_TOOLS_HPP_ */
