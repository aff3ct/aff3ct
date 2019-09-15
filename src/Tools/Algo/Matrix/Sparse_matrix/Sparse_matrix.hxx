#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
const std::vector<uint32_t>& Sparse_matrix
::get_cols_from_row(const size_t row_index) const
{
	return this->row_to_cols[row_index];
}

const std::vector<uint32_t>& Sparse_matrix
::get_rows_from_col(const size_t col_index) const
{
	return this->col_to_rows[col_index];
}

const std::vector<uint32_t>& Sparse_matrix
::operator[](const size_t col_index) const
{
	return this->get_rows_from_col(col_index);
}

const std::vector<std::vector<uint32_t>>& Sparse_matrix
::get_row_to_cols() const
{
	return this->row_to_cols;
}

const std::vector<std::vector<uint32_t>>& Sparse_matrix
::get_col_to_rows() const
{
	return this->col_to_rows;
}
}
}
