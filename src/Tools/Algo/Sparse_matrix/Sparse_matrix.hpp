#ifndef SPARSE_MATRIX_HPP_
#define SPARSE_MATRIX_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace aff3ct
{
namespace tools
{
class Sparse_matrix
{
private:
	unsigned n_rows;
	unsigned n_cols;
	unsigned rows_max_degree;
	unsigned cols_max_degree;
	unsigned n_connections;

	std::vector<std::vector<unsigned>> row_to_cols;
	std::vector<std::vector<unsigned>> col_to_rows;

public:
	Sparse_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Sparse_matrix();

	inline unsigned get_n_rows() const
	{
		return this->n_rows;
	}

	inline unsigned get_n_cols() const
	{
		return this->n_cols;
	}

	inline unsigned get_rows_max_degree() const
	{
		return this->rows_max_degree;
	}

	inline unsigned get_cols_max_degree() const
	{
		return this->cols_max_degree;
	}

	inline unsigned get_n_connections() const
	{
		return this->n_connections;
	}

	const std::vector<unsigned>& get_cols_from_row(const size_t row_index) const
	{
		return this->row_to_cols[row_index];
	}

	const std::vector<unsigned>& get_rows_from_col(const size_t col_index) const
	{
		return this->col_to_rows[col_index];
	}

	inline const std::vector<unsigned>& operator[](const size_t col_index) const
	{
		return this->get_rows_from_col(col_index);
	}

	inline bool at(const size_t row_index, const size_t col_index) const
	{
		auto it = std::find(this->row_to_cols[row_index].begin(), this->row_to_cols[row_index].end(), col_index);
		return (it != this->row_to_cols[row_index].end());
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
	 * Return the density of ones in this matrix
	 */
	float compute_density() const;

	/*
	 * Sort the sparse matrix per density of lines
	 * The "order" parameter can be "ASC" for ascending or "DSC" for descending
	 */
	void sort_cols_per_density(std::string order = "DSC");

	/*
	 * Print the sparsed matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	void print(bool transpose = false, std::ostream& os = std::cout) const;
};
}
}

std::ostream& operator<<(std::ostream& os, const aff3ct::tools::Sparse_matrix& sm);

#endif /* SPARSE_MATRIX_HPP_ */
