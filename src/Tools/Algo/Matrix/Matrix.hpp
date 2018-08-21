#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace aff3ct
{
namespace tools
{
class Matrix
{
private:
	unsigned n_rows;
	unsigned n_cols;

protected:
	unsigned rows_max_degree;
	unsigned cols_max_degree;
	unsigned n_connections;

public:
	enum class Way
	{
		HORIZONTAL, // meaning longest side depicts columns,
		VERTICAL    // meaning longest side depicts rows
	};

	enum class Sort
	{
		ASCENDING, // meaning longest side depicts columns,
		DESCENDING    // meaning longest side depicts rows
	};

	Matrix(const unsigned n_rows = 0, const unsigned n_cols = 1);

	virtual ~Matrix() = default;

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

	/*
	 * return true if there is a connexion there
	 */
	virtual bool at(const size_t row_index, const size_t col_index) const = 0;

	/*
	 * Check id indexes are in the matrix bounds
	 */
	void check_indexes(const size_t row_index, const size_t col_index) const;

	/*
	 * Add a connection
	 */
	virtual void add_connection(const size_t row_index, const size_t col_index) = 0;

	/*
	 * Remove the connection
	 */
	virtual void rm_connection(const size_t row_index, const size_t col_index) = 0;

	/*
	 * Transpose internally this matrix
	 */
	virtual void self_transpose();

	/*
	 * Return turn the matrix in horizontal or vertical way
	 */
	void self_turn(Way w);

	/*
	 * Return the density of ones in this matrix
	 */
	float compute_density() const;

	/*
	 * Sort the matrix per density of lines in ascending or descending order
	 */
	virtual void sort_cols_per_density(Sort s) = 0;

	/*
	 * Print the sparsed matrix in its full view with 0s and 1s.
	 * 'transpose' allow the print in its transposed view
	 */
	virtual void print(bool transpose = false, std::ostream& os = std::cout) const = 0;
};
}
}

std::ostream& operator<<(std::ostream& os, const aff3ct::tools::Matrix& sm);

#endif /* MATRIX_HPP_ */
