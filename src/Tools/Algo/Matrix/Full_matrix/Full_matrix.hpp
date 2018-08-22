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
/*
 * \param T must be uint8_t or uint16_t or uint32_t or uint64_t
 */

template <typename T = uint32_t>
class Full_matrix : public Matrix
{
public:
	Full_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Full_matrix() = default;

	inline const std::vector<T>& get_cols_from_row(const size_t row_index) const
	{
		return values[row_index];
	}

	inline const std::vector<T>& operator[](const size_t row_index) const
	{
		return get_cols_from_row(row_index);
	}

	inline std::vector<T>& get_cols_from_row(const size_t row_index)
	{
		return values[row_index];
	}

	inline std::vector<T>& operator[](const size_t row_index)
	{
		return get_cols_from_row(row_index);
	}

	/*
	 * return true if there is a connection there
	 */
	bool at(const size_t row_index, const size_t col_index) const;

	/*
	 * Add a connection and update the rows and cols degrees values
	 */
	void add_connection(const size_t row_index, const size_t col_index);

	/*
	 * Remove the connection and update the rows and cols degrees values
	 */
	void rm_connection(const size_t row_index, const size_t col_index);

	/*
	 * Compute the rows and cols degrees values when the matrix values have been modified
	 * without the use of 'add_connection' and 'rm_connection' interface
	 */
	void parse_connections();

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
	void sort_cols_per_density(Sort order);

	/*
	 * Print the matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	void print(bool transpose = false, std::ostream& os = std::cout) const;

private:
	std::vector<std::vector<T>> values;

	std::vector<size_t> rows_degrees;
	std::vector<size_t> cols_degrees;
};
}
}

#endif /* FULL_MATRIX_HPP_ */
