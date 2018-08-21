#ifndef SPARSE_MATRIX_HPP_
#define SPARSE_MATRIX_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "../Matrix.hpp"

namespace aff3ct
{
namespace tools
{
class Sparse_matrix : public Matrix
{
private:
	std::vector<std::vector<unsigned>> row_to_cols;
	std::vector<std::vector<unsigned>> col_to_rows;

public:

	Sparse_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Sparse_matrix() = default;

	inline const std::vector<unsigned>& get_cols_from_row(const size_t row_index) const
	{
		return this->row_to_cols[row_index];
	}

	inline const std::vector<unsigned>& get_rows_from_col(const size_t col_index) const
	{
		return this->col_to_rows[col_index];
	}

	inline const std::vector<unsigned>& operator[](const size_t col_index) const
	{
		return this->get_rows_from_col(col_index);
	}

	inline const std::vector<std::vector<unsigned>>& get_row_to_cols() const
	{
		return this->row_to_cols;
	}

	inline const std::vector<std::vector<unsigned>>& get_col_to_rows() const
	{
		return this->col_to_rows;
	}

	/*
	 * return true if there is a connexion there
	 */
	bool at(const size_t row_index, const size_t col_index) const;

	/*
	 * Add a connection
	 */
	void add_connection(const size_t row_index, const size_t col_index);

	/*
	 * Remove the connection
	 */
	void rm_connection(const size_t row_index, const size_t col_index);

	/*
	 * Return the transposed matrix of this matrix
	 */
	Sparse_matrix transpose() const;

	/*
	 * Transpose internally this matrix
	 */
	void self_transpose();

	/*
	 * Return turn the matrix in horizontal or vertical way
	 */
	Sparse_matrix turn(Way w) const;

	/*
	 * Sort the sparse matrix per density of lines
	 * The "order" parameter can be "ASC" for ascending or "DSC" for descending
	 */
	void sort_cols_per_density(Sort s);

	/*
	 * Print the sparsed matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	void print(bool transpose = false, std::ostream& os = std::cout) const;
};
}
}
#endif /* SPARSE_MATRIX_HPP_ */
