#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Full_matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Full_matrix<T>
::Full_matrix(const unsigned n_rows, const unsigned n_cols)
: Matrix(n_rows, n_cols),
  values(n_rows, std::vector<T>(n_cols,0)),
  rows_degrees(n_rows,0),
  cols_degrees(n_cols,0)
{
}

template <typename T>
bool Full_matrix<T>
::at(const size_t row_index, const size_t col_index) const
{
	check_indexes(row_index, col_index);

	return values[row_index][col_index] != 0;
}

template <typename T>
void Full_matrix<T>
::add_connection(const size_t row_index, const size_t col_index)
{
	if (at(row_index, col_index))
	{
		std::stringstream message;
		message << "('row_index';'col_index') connection already exists ('row_index' = " << row_index
		        << ", 'col_index' = " << col_index << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	values[row_index][col_index] = 1;
	rows_degrees[row_index]++;
	cols_degrees[col_index]++;

	this->rows_max_degree = std::max(get_rows_max_degree(), rows_degrees[row_index]);
	this->cols_max_degree = std::max(get_cols_max_degree(), cols_degrees[col_index]);

	this->n_connections++;
}

template <typename T>
void Full_matrix<T>
::rm_connection(const size_t row_index, const size_t col_index)
{
	if(!at(row_index, col_index))
		return;

	values[row_index][col_index] = 0;

	rows_degrees[row_index]--;
	cols_degrees[col_index]--;
	this->n_connections--;

	if ((rows_degrees[row_index] + 1) == get_rows_max_degree())
		rows_max_degree = *std::max_element(rows_degrees.begin(), rows_degrees.end());

	if ((cols_degrees[col_index] + 1) == get_cols_max_degree())
		cols_max_degree = *std::max_element(cols_degrees.begin(), cols_degrees.end());
}

template <typename T>
void Full_matrix<T>
::parse_connections()
{
	std::fill(rows_degrees.begin(), rows_degrees.end(), 0);
	std::fill(cols_degrees.begin(), cols_degrees.end(), 0);
	this->n_connections = 0;

	for (size_t r = 0; r < get_n_rows(); r++)
		for (size_t c = 0; c < get_n_cols(); c++)
			if (values[r][c])
			{
				rows_degrees[r]++;
				cols_degrees[c]++;
				this->n_connections++;
			}

	rows_max_degree = *std::max_element(rows_degrees.begin(), rows_degrees.end());
	cols_max_degree = *std::max_element(cols_degrees.begin(), cols_degrees.end());
}

template <typename T>
Full_matrix<T> Full_matrix<T>
::transpose() const
{
	Full_matrix trans(*this);

	trans.self_transpose();

	return trans;
}

template <typename T>
void Full_matrix<T>
::self_transpose()
{
	// extend the vector to allow the swap of values and then resize it to the new transposed size
	switch(get_way())
	{
		case Way::HORIZONTAL:
			values.resize(get_n_cols(), std::vector<T>(get_n_rows()));

			for (size_t r = 0; r < get_n_rows(); r++)
				for (size_t c = r + 1; c < get_n_rows(); c++)
					std::swap(values[r][c], values[c][r]);

			for (size_t r = get_n_rows(); r < get_n_cols(); r++)
				for (size_t c = 0; c < get_n_rows(); c++)
					values[r][c] = values[c][r];

			for (size_t r = 0; r < get_n_rows(); r++)
				values[r].resize(get_n_rows());
		break;
		case Way::VERTICAL:
			for (size_t r = 0; r < get_n_cols(); r++)
				values[r].resize(get_n_rows());

			for (size_t r = 0; r < get_n_cols(); r++)
				for (size_t c = r + 1; c < get_n_cols(); c++)
					std::swap(values[r][c], values[c][r]);

			for (size_t r = get_n_cols(); r < get_n_rows(); r++)
				for (size_t c = 0; c < get_n_cols(); c++)
					values[c][r] = values[r][c];

			values.resize(get_n_cols());
		break;
	}

	Matrix::self_transpose(); // transpose the matrix size and degrees

	std::swap(cols_degrees, rows_degrees);

}

template <typename T>
Full_matrix<T> Full_matrix<T>
::turn(Way w) const
{
	Full_matrix turned(*this);

	turned.self_turn(w);

	return turned;
}

template <typename T>
void Full_matrix<T>
::sort_cols_per_density(Sort order)
{
	switch(order)
	{
		case Sort::ASCENDING:
			tools::mutual_sort(cols_degrees, values, [](size_t i1, size_t i2) { return i1 < i2; });
		break;
		case Sort::DESCENDING:
			tools::mutual_sort(cols_degrees, values, [](size_t i1, size_t i2) { return i1 > i2; });
		break;
	}
}

template <typename T>
void Full_matrix<T>
::print(bool transpose, std::ostream& os) const
{
	if (transpose)
	{
		for (size_t c = 0; c < get_n_cols(); c++)
		{
			for (size_t r = 0; r < get_n_rows(); r++)
				os << +values[r][c] << " ";
			os << std::endl;
		}
	}
	else
	{
		for (size_t r = 0; r < get_n_rows(); r++)
		{
			for (size_t c = 0; c < get_n_cols(); c++)
				os << +values[r][c] << " ";
			os << std::endl;
		}
	}
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Full_matrix< uint8_t>;
template class aff3ct::tools::Full_matrix<uint16_t>;
template class aff3ct::tools::Full_matrix<uint32_t>;
template class aff3ct::tools::Full_matrix<uint64_t>;
// ==================================================================================== explicit template instantiation
