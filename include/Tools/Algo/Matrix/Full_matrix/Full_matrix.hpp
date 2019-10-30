/*!
 * \file
 * \brief Class tools::Full_matrix.
 */
#ifndef FULL_MATRIX_HPP_
#define FULL_MATRIX_HPP_

#include <iostream>
#include <cstddef>
#include <vector>

#include "Tools/Algo/Matrix/Matrix.hpp"

namespace aff3ct
{
namespace tools
{
/*
 * \param T must be int8_t or int16_t or int32_t or int64_t
 * Warning ! Never call modification methods (resize, erase, push_back and so on), on the second dimension of the Full_matrix
 *   always pass through Full_matrix methods, else you may obtain runtime errors
 */

template <typename T = int32_t>
class Full_matrix : public Matrix, private std::vector<std::vector<T>>
{
public:
	using Container = std::vector<std::vector<T>>;
	using value_type = T;

	explicit Full_matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Full_matrix() = default;

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
	 * Resize the matrix to the new dimensions in function of the part of the matrix considered as the origin
	 * Ex: if o == BOTTOM_RIGHT, and need to extend the matrix then add rows on the top and columns on the left
	 */
	void self_resize(const size_t n_rows, const size_t n_cols, Origin o);

	/*
	 * Resize the matrix by calling self_resize on a copy matrix
	 */
	Full_matrix resize(const size_t n_rows, const size_t n_cols, Origin o) const;


	/*
	 * Erase the 'n_rows' rows from the given index 'row_index'
	 */
	void erase_row(const size_t row_index, const size_t n_rows = 1);

	/*
	 * Erase the 'n_cols' cols from the given index 'col_index'
	 */
	void erase_col(const size_t col_index, const size_t n_cols = 1);


	using Container::size;
	using Container::operator[];

	/*
	 * Compute the rows and cols degrees values when the matrix values have been modified
	 * without the use of 'add_connection' and 'rm_connection' interface, so after any modification
	 * call this function if you need degrees information
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
	 * You need to call 'parse_connections()' before to assure good work.
	 */
	void sort_cols_per_density(Sort order);

	/*
	 * Print the matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	void print(bool transpose = false, std::ostream& os = std::cout) const;


	/*
	 * \brief create a matrix of the given size filled with identity diagonal
	 * \return the identity matrix
	 */
	static Full_matrix<T> identity(const size_t n_rows, const size_t n_cols);

	/*
	 * \brief create a matrix of the given size filled with only zeros
	 * \return the zero matrix
	 */
	static Full_matrix<T> zero(const size_t n_rows, const size_t n_cols);

private:
	std::vector<size_t> rows_degrees;
	std::vector<size_t> cols_degrees;
};
}
}

#endif /* FULL_MATRIX_HPP_ */
