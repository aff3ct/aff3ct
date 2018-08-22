#include <string>
#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"

#include "Matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Matrix
::Matrix(const size_t n_rows, const size_t n_cols)
: n_rows         (n_rows),
  n_cols         (n_cols),
  rows_max_degree(0     ),
  cols_max_degree(0     ),
  n_connections  (0     )
{
}

void Matrix
::self_transpose()
{
	std::swap(n_rows,          n_cols         );
	std::swap(rows_max_degree, cols_max_degree);
}

void Matrix
::self_turn(Way w)
{
	if (w == Way::VERTICAL)
	{
		if (get_n_cols() > get_n_rows())
			this->self_transpose();
	}
	else if (w == Way::HORIZONTAL)
	{
		if (get_n_cols() < get_n_rows())
			this->self_transpose();
	}
}

Matrix::Way Matrix
::get_way() const
{
	return (get_n_cols() >= get_n_rows()) ? Way::HORIZONTAL : Way::VERTICAL;
}

float Matrix
::compute_density() const
{
	return ((float)n_connections / (float)(n_rows * n_cols));
}

void Matrix
::check_indexes(const size_t row_index, const size_t col_index) const
{
	if (col_index >= get_n_cols())
	{
		std::stringstream message;
		message << "'col_index' has to be smaller than 'n_cols' ('col_index' = " << col_index
		        << ", 'n_cols' = " << get_n_cols() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (row_index >= get_n_rows())
	{
		std::stringstream message;
		message << "'row_index' has to be smaller than 'n_rows' ('row_index' = " << row_index
		        << ", 'n_rows' = " << get_n_rows() << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}


std::ostream& operator<<(std::ostream& os, const Matrix& sm)
{
	sm.print(0, os);
	return os;
}

