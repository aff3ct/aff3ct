#ifndef SPARSE_MATRIX_HPP_
#define SPARSE_MATRIX_HPP_

#include <vector>
#include <string>

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

	unsigned get_n_rows         () const;
	unsigned get_n_cols         () const;
	unsigned get_rows_max_degree() const;
	unsigned get_cols_max_degree() const;
	unsigned get_n_connections  () const;

	const std::vector<unsigned>& get_cols_from_row(const size_t row_index) const;
	const std::vector<unsigned>& get_rows_from_col(const size_t col_index) const;

	const std::vector<unsigned>& operator[](const size_t col_index) const;

	bool at(const size_t row_index, const size_t col_index) const;

	const std::vector<std::vector<unsigned int>>& get_row_to_cols() const;
	const std::vector<std::vector<unsigned int>>& get_col_to_rows() const;

	void add_connection(const size_t row_index, const size_t col_index);

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
};
}
}

#endif /* SPARSE_MATRIX_HPP_ */
