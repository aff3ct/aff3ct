#ifndef FULL_MATRIX_HPP_
#define FULL_MATRIX_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "../Matrix.hpp"

namespace aff3ct
{
namespace tools
{
class Full_matrix : public Matrix
{
private:
	std::vector<std::vector<unsigned>> values;

	std::vector<unsigned> rows_degrees;
	std::vector<unsigned> cols_degrees;

public:

	Full_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Full_matrix() = default;

	inline const std::vector<unsigned>& get_cols_from_row(const size_t row_index) const
	{
		return values[row_index];
	}

	inline const std::vector<unsigned>& operator[](const size_t row_index) const
	{
		return get_cols_from_row(row_index);
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
	Full_matrix transpose() const;

	/*
	 * Transpose internally this matrix
	 */
	void self_transpose();

	/*
	 * Return turn the matrix in horizontal or vertical way
	 */
	Full_matrix turn(Way w) const;

	/*
	 * Sort the matrix per density of lines in ascending or descending order
	 */
	void sort_cols_per_density(Sort s);

	/*
	 * Print the matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	void print(bool transpose = false, std::ostream& os = std::cout) const;
};
}
}

#endif /* FULL_MATRIX_HPP_ */
