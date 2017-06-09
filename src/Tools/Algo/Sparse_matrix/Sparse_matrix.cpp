#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "Sparse_matrix.hpp"

using namespace aff3ct;
using namespace tools;

Sparse_matrix
::Sparse_matrix(const unsigned n_rows, const unsigned n_cols)
: n_rows         (n_rows),
  n_cols         (n_cols),
  rows_max_degree(0     ),
  cols_max_degree(0     ),
  n_connections  (0     ),
  row_to_cols    (n_rows),
  col_to_rows    (n_cols)
{
}

Sparse_matrix
::~Sparse_matrix()
{
}

unsigned Sparse_matrix
::get_n_rows() const
{
	return this->n_rows;
}

unsigned Sparse_matrix
::get_n_cols() const
{
	return this->n_cols;
}

unsigned Sparse_matrix
::get_rows_max_degree() const
{
	return this->rows_max_degree;
}

unsigned Sparse_matrix
::get_cols_max_degree() const
{
	return this->cols_max_degree;
}

unsigned Sparse_matrix
::get_n_connections() const
{
	return this->n_connections;
}

const std::vector<unsigned>& Sparse_matrix
::get_cols_from_row(const size_t row_index) const
{
	return this->row_to_cols[row_index];
}

const std::vector<unsigned>& Sparse_matrix
::get_rows_from_col(const size_t col_index) const
{
	return this->col_to_rows[col_index];
}

const std::vector<unsigned>& Sparse_matrix
::operator[](const size_t col_index) const
{
	return this->get_rows_from_col(col_index);
}

bool Sparse_matrix
::at(const size_t row_index, const size_t col_index) const
{
	auto it = std::find(this->row_to_cols[row_index].begin(), this->row_to_cols[row_index].end(), col_index);

	return (it != this->row_to_cols[row_index].end());
}

const std::vector<std::vector<unsigned int>>& Sparse_matrix
::get_row_to_cols() const
{
	return this->row_to_cols;
}

const std::vector<std::vector<unsigned int>>& Sparse_matrix
::get_col_to_rows() const
{
	return this->col_to_rows;
}

void Sparse_matrix
::add_connection(const size_t row_index, const size_t col_index)
{
	if (col_index >= this->n_cols)
		throw std::invalid_argument("aff3ct::tools::Sparse_matrix: \"col_index\" has to be smaller than \"cols\".");

	if (row_index >= this->n_rows)
		throw std::invalid_argument("aff3ct::tools::Sparse_matrix: \"row_index\" has to be smaller than \"rows\".");

	for (size_t i = 0; i < this->row_to_cols[row_index].size(); i++)
		if (this->row_to_cols[row_index][i] == col_index)
			throw std::runtime_error("aff3ct::tools::Sparse_matrix: \"col_index\" already exists.");

	for (size_t i = 0; i < this->col_to_rows[col_index].size(); i++)
		if (this->col_to_rows[col_index][i] == row_index)
			throw std::runtime_error("aff3ct::tools::Sparse_matrix: \"row_index\" already exists.");

	this->row_to_cols[row_index].push_back((unsigned)col_index);
	this->col_to_rows[col_index].push_back((unsigned)row_index);

	rows_max_degree = std::max(rows_max_degree, (unsigned)this->row_to_cols[row_index].size());
	cols_max_degree = std::max(cols_max_degree, (unsigned)this->col_to_rows[col_index].size());

	this->n_connections++;
}

Sparse_matrix Sparse_matrix
::transpose() const
{
	Sparse_matrix trans(*this);

	trans.self_transpose();

	return trans;
}

void Sparse_matrix
::self_transpose()
{
	std::swap(this->n_rows,          this->n_cols         );
	std::swap(this->rows_max_degree, this->cols_max_degree);
	std::swap(this->row_to_cols,     this->col_to_rows    );
}

float Sparse_matrix
::compute_density() const
{
	return ((float)n_connections/(float)(n_rows*n_cols));
}
