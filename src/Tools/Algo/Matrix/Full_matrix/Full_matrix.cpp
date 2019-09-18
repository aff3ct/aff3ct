#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Tools/Algo/Matrix/Full_matrix/Full_matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Full_matrix<T>
::Full_matrix(const unsigned n_rows, const unsigned n_cols)
: Matrix(n_rows, n_cols),
  std::vector<std::vector<T>>(n_rows, std::vector<T>(n_cols,0)),
  rows_degrees(n_rows,0),
  cols_degrees(n_cols,0)
{
}

template <typename T>
bool Full_matrix<T>
::at(const size_t row_index, const size_t col_index) const
{
	check_indexes(row_index, col_index);

	return (*this)[row_index][col_index] != 0;
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

	(*this)[row_index][col_index] = 1;
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

	(*this)[row_index][col_index] = 0;

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
::self_resize(const size_t n_rows, const size_t n_cols, Origin o)
{
	const auto min_r = std::min(n_rows, get_n_rows());

	switch(o)
	{
		case Origin::TOP_LEFT:
		{
			Container::resize(n_rows, std::vector<T>(n_cols, 0));
			for (size_t r = 0; r < min_r; r++)
				(*this)[r].resize(n_cols, 0);
		}
		break;
		case Origin::TOP_RIGHT:
		{
			Container::resize(n_rows, std::vector<T>(n_cols, 0));
			if (n_cols < get_n_cols())
			{
				auto n_erase = get_n_cols() - n_cols;
				for (size_t r = 0; r < n_rows; r++)
					(*this)[r].erase((*this)[r].begin(), (*this)[r].begin() + n_erase);
			}
			else
			{
				auto n_insert = n_cols - get_n_cols();
				for (size_t r = 0; r < min_r; r++)
					(*this)[r].insert((*this)[r].begin(), n_insert, 0);
			}
		}
		break;
		case Origin::BOTTOM_LEFT:
		{
			if (n_rows < get_n_rows())
			{
				auto n_erase = get_n_rows() - n_rows;
				Container::erase(Container::begin(), Container::begin() + n_erase);

				for (size_t r = 0; r < min_r; r++)
					(*this)[r].resize(n_cols, 0);
			}
			else
			{
				auto n_insert = n_rows - get_n_rows();
				Container::insert(Container::begin(), n_insert, std::vector<T>(n_cols, 0));

				for (size_t r = n_insert; r < n_rows; r++)
					(*this)[r].resize(n_cols, 0);
			}
		}
		break;
		case Origin::BOTTOM_RIGHT:
		{
			if (n_rows < get_n_rows())
			{
				auto n_erase = get_n_rows() - n_rows;
				Container::erase(Container::begin(), Container::begin() + n_erase);

				if (n_cols < get_n_cols())
				{
					auto n_erase = get_n_cols() - n_cols;
					for (size_t r = 0; r < n_rows; r++)
						(*this)[r].erase((*this)[r].begin(), (*this)[r].begin() + n_erase);
				}
				else
				{
					auto n_insert = n_cols - get_n_cols();
					for (size_t r = 0; r < n_rows; r++)
						(*this)[r].insert((*this)[r].begin(), n_insert, 0);
				}
			}
			else
			{
				auto n_insert = n_rows - get_n_rows();
				Container::insert(Container::begin(), n_insert, std::vector<T>(n_cols, 0));

				if (n_cols < get_n_cols())
				{
					auto n_erase = get_n_cols() - n_cols;
					for (size_t r = n_insert; r < n_rows; r++)
						(*this)[r].erase((*this)[r].begin(), (*this)[r].begin() + n_erase);
				}
				else
				{
					auto n_insert2 = n_cols - get_n_cols();
					for (size_t r = n_insert; r < n_rows; r++)
						(*this)[r].insert((*this)[r].begin(), n_insert2, 0);
				}
			}
		}
		break;
	}

	Matrix::self_resize(n_rows, n_cols);

	rows_degrees.resize(n_rows);
	cols_degrees.resize(n_rows);
}

template <typename T>
Full_matrix<T> Full_matrix<T>
::resize(const size_t n_rows, const size_t n_cols, Origin o) const
{
	Full_matrix resized(*this);

	resized.self_resize(n_rows, n_cols, o);

	return resized;
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
			if ((*this)[r][c])
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
	// extend the vector to allow the swap of (*this) and then resize it to the new transposed size
	switch(get_way())
	{
		case Way::HORIZONTAL:
			Container::resize(get_n_cols(), std::vector<T>(get_n_rows()));

			for (size_t r = 0; r < get_n_rows(); r++)
				for (size_t c = r + 1; c < get_n_rows(); c++)
					std::swap((*this)[r][c], (*this)[c][r]);

			for (size_t r = get_n_rows(); r < get_n_cols(); r++)
				for (size_t c = 0; c < get_n_rows(); c++)
					(*this)[r][c] = (*this)[c][r];

			for (size_t r = 0; r < get_n_rows(); r++)
				(*this)[r].resize(get_n_rows());
		break;
		case Way::VERTICAL:
			for (size_t r = 0; r < get_n_cols(); r++)
				(*this)[r].resize(get_n_rows());

			for (size_t r = 0; r < get_n_cols(); r++)
				for (size_t c = r + 1; c < get_n_cols(); c++)
					std::swap((*this)[r][c], (*this)[c][r]);

			for (size_t r = get_n_cols(); r < get_n_rows(); r++)
				for (size_t c = 0; c < get_n_cols(); c++)
					(*this)[c][r] = (*this)[r][c];

			Container::resize(get_n_cols());
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
			tools::mutual_sort(cols_degrees, (*this), [](size_t i1, size_t i2) { return i1 < i2; });
		break;
		case Sort::DESCENDING:
			tools::mutual_sort(cols_degrees, (*this), [](size_t i1, size_t i2) { return i1 > i2; });
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
				os << +(*this)[r][c] << " ";
			os << std::endl;
		}
	}
	else
	{
		for (size_t r = 0; r < get_n_rows(); r++)
		{
			for (size_t c = 0; c < get_n_cols(); c++)
				os << +(*this)[r][c] << " ";
			os << std::endl;
		}
	}
}


template <typename T>
Full_matrix<T> Full_matrix<T>
::identity(const size_t n_rows, const size_t n_cols)
{
	Full_matrix<T> mat((unsigned)n_rows, (unsigned)n_cols);
	auto shortest_side = std::min(n_rows, n_cols);

	for (size_t i = 0; i < shortest_side; i++)
		mat.add_connection(i,i);

	return mat;
}

template <typename T>
Full_matrix<T> Full_matrix<T>
::zero(const size_t n_rows, const size_t n_cols)
{
	return Full_matrix<T>((unsigned)n_rows, (unsigned)n_cols);
}

template <typename T>
void Full_matrix<T>
::erase_row(const size_t row_index, const size_t n_rows)
{
	Container::erase(Container::begin() + row_index, Container::begin() + row_index + n_rows);
	rows_degrees.erase(rows_degrees.begin() + row_index,rows_degrees.begin() + row_index + n_rows);

	Matrix::self_resize(get_n_rows() - n_rows, get_n_cols());
}

template <typename T>
void Full_matrix<T>
::erase_col(const size_t col_index, const size_t n_cols)
{
	for (size_t i = 0; i < get_n_rows(); i++)
	{
		auto& row = (*this)[i];
		row.erase(row.begin() + col_index, row.begin() + col_index + n_cols);
	}

	cols_degrees.erase(cols_degrees.begin() + col_index, cols_degrees.begin() + col_index + n_cols);

	Matrix::self_resize(get_n_rows(), get_n_cols() - n_cols);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Full_matrix< int8_t>;
template class aff3ct::tools::Full_matrix<int16_t>;
template class aff3ct::tools::Full_matrix<int32_t>;
template class aff3ct::tools::Full_matrix<int64_t>;
// ==================================================================================== explicit template instantiation
