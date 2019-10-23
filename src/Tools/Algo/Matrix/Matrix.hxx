#include "Tools/Algo/Matrix/Matrix.hpp"

namespace aff3ct
{
namespace tools
{
size_t Matrix
::get_n_rows() const
{
	return this->n_rows;
}

size_t Matrix
::get_n_cols() const
{
	return this->n_cols;
}

size_t Matrix
::size() const
{
	return this->n_rows * this->n_cols;
}

size_t Matrix
::get_rows_max_degree() const
{
	return this->rows_max_degree;
}

size_t Matrix
::get_cols_max_degree() const
{
	return this->cols_max_degree;
}

size_t Matrix
::get_n_connections() const
{
	return this->n_connections;
}
}
}
