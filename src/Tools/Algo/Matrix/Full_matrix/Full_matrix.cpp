#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Full_matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Full_matrix
::Full_matrix(const unsigned n_rows, const unsigned n_cols)
: Matrix(n_rows, n_cols),
  values(n_rows, std::vector<unsigned>(n_cols,0)),
  rows_degrees(n_rows,0),
  cols_degrees(n_cols,0)
{
}

bool Full_matrix
::at(const size_t row_index, const size_t col_index) const
{
	check_indexes(row_index, col_index);

	return values[row_index][col_index] != 0;
}

void Full_matrix
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

void Full_matrix::rm_connection(const size_t row_index, const size_t col_index)
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

Full_matrix Full_matrix
::transpose() const
{
	Full_matrix trans(*this);

	trans.self_transpose();

	return trans;
}

void Full_matrix
::self_transpose()
{
	Matrix::self_transpose();

	std::swap(cols_degrees, rows_degrees);
}

Full_matrix Full_matrix
::turn(Way w) const
{
	Full_matrix turned(*this);

	turned.self_turn(w);

	return turned;
}

void Full_matrix
::sort_cols_per_density(Sort s)
{
	// if (order == Sort::ASCENDING)
	// {
	// 	std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
	// 	          [](const std::vector<unsigned> &i1, const  std::vector<unsigned> &i2) { return i1.size() < i2.size(); });
	// }
	// else // order == "DSC"
	// {
	// 	std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
	// 	          [](const  std::vector<unsigned> &i1, const std::vector<unsigned> &i2) { return i1.size() > i2.size(); });
	// }

	// for (auto &r : this->row_to_cols)
	// 	r.clear();
	// for (size_t i = 0; i < this->col_to_rows.size(); i++)
	// 	for (size_t j = 0; j < this->col_to_rows[i].size(); j++)
	// 		this->row_to_cols[this->col_to_rows[i][j]].push_back((unsigned)i);
}

void Full_matrix
::print(bool transpose, std::ostream& os) const
{
	// if (transpose)
	// {
	// 	std::vector<unsigned> rows(this->n_rows, 0);

	// 	for (auto& col : this->col_to_rows)
	// 	{
	// 		// set the ones
	// 		for (auto& row : col)
	// 			rows[row] = 1;

	// 		for (auto& row : rows)
	// 			os << row << " ";

	// 		os << std::endl;

	// 		// reset the ones
	// 		for (auto& row : col)
	// 			rows[row] = 0;
	// 	}
	// }
	// else
	// {
	// 	std::vector<unsigned> columns(this->n_cols, 0);

	// 	for (auto& row : this->row_to_cols)
	// 	{
	// 		// set the ones
	// 		for (auto& col : row)
	// 			columns[col] = 1;

	// 		for (auto& col : columns)
	// 			os << col << " ";

	// 		os << std::endl;

	// 		// reset the ones
	// 		for (auto& col : row)
	// 			columns[col] = 0;
	// 	}
	// }
}
