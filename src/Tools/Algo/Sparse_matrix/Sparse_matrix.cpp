#include <string>
#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"

#include "Sparse_matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

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

void Sparse_matrix
::add_connection(const size_t row_index, const size_t col_index)
{
	if (col_index >= this->n_cols)
	{
		std::stringstream message;
		message << "'col_index' has to be smaller than 'n_cols' ('col_index' = " << col_index
		        << ", 'n_cols' = " << this->n_cols << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (row_index >= this->n_rows)
	{
		std::stringstream message;
		message << "'row_index' has to be smaller than 'n_rows' ('row_index' = " << row_index
		        << ", 'n_rows' = " << this->n_rows << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t i = 0; i < this->row_to_cols[row_index].size(); i++)
		if (this->row_to_cols[row_index][i] == col_index)
		{
			std::stringstream message;
			message << "'col_index' already exists ('col_index' = " << col_index << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	for (size_t i = 0; i < this->col_to_rows[col_index].size(); i++)
		if (this->col_to_rows[col_index][i] == row_index)
		{
			std::stringstream message;
			message << "'row_index' already exists ('row_index' = " << row_index << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

	this->row_to_cols[row_index].push_back((unsigned)col_index);
	this->col_to_rows[col_index].push_back((unsigned)row_index);

	rows_max_degree = std::max(rows_max_degree, (unsigned)this->row_to_cols[row_index].size());
	cols_max_degree = std::max(cols_max_degree, (unsigned)this->col_to_rows[col_index].size());

	this->n_connections++;
}

void Sparse_matrix::rm_connection(const size_t row_index, const size_t col_index)
{
	if (col_index >= this->n_cols)
	{
		std::stringstream message;
		message << "'col_index' has to be smaller than 'n_cols' ('col_index' = " << col_index
		        << ", 'n_cols' = " << this->n_cols << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (row_index >= this->n_rows)
	{
		std::stringstream message;
		message << "'row_index' has to be smaller than 'n_rows' ('row_index' = " << row_index
		        << ", 'n_rows' = " << this->n_rows << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// delete the link in the row_to_cols vector
	bool row_found = false;
	auto itr = std::find(this->row_to_cols[row_index].begin(), this->row_to_cols[row_index].end(), col_index);
	if (itr != this->row_to_cols[row_index].end())
	{
		row_found = true;
		itr = this->row_to_cols[row_index].erase(itr);

		// check if need to reduce the rows max degree
		if (this->row_to_cols[row_index].size() == (this->rows_max_degree-1))
		{
			bool found = false;
			for (auto i = this->row_to_cols.begin(); i != this->row_to_cols.end(); i++ )
				if (i->size() == this->rows_max_degree)
				{
					found = true;
					break;
				}

			if (!found)
				this->rows_max_degree--;
		}
	}

	// delete the link in the col_to_rows vector
	bool col_found = false;
	auto itc = std::find(this->col_to_rows[col_index].begin(), this->col_to_rows[col_index].end(), row_index);
	if (itc != this->col_to_rows[col_index].end())
	{
		col_found = true;
		this->col_to_rows[col_index].erase(itc);

		// check if need to reduce the cols max degree
		if (this->col_to_rows[col_index].size() == (this->cols_max_degree-1))
		{
			bool found = false;
			for (auto i = this->col_to_rows.begin(); i != this->col_to_rows.end(); i++ )
				if (i->size() == this->cols_max_degree)
				{
					found = true;
					break;
				}

			if (!found)
				this->cols_max_degree--;
		}
	}

	if (row_found != col_found)
	{
		std::stringstream message;
		message << "The connection has been found only in one of the two vectors 'row_to_cols' and 'col_to_rows' "
		        << "('row_index' = " << row_index << ", 'col_index' = " << col_index
		        << ", found in row = " << std::boolalpha << row_found
		        << ", found in col = " << col_found << std::noboolalpha << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
	else if (row_found && col_found)
		this->n_connections--;
	// else the connection has not been found
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
	return ((float)n_connections / (float)(n_rows * n_cols));
}

void Sparse_matrix
::sort_cols_per_density(std::string order)
{
	if (order != "ASC" && order != "DSC")
	{
		std::stringstream message;
		message << "'order' is unsupported, it sould be 'ASC' or 'DSC' ('order' = " << order << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (order == "ASC")
	{
		std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
		          [](const std::vector<unsigned> &i1, const  std::vector<unsigned> &i2) { return i1.size() < i2.size(); });
	}
	else // order == "DSC"
	{
		std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
		          [](const  std::vector<unsigned> &i1, const std::vector<unsigned> &i2) { return i1.size() > i2.size(); });
	}

	for (auto &r : this->row_to_cols)
		r.clear();
	for (size_t i = 0; i < this->col_to_rows.size(); i++)
		for (size_t j = 0; j < this->col_to_rows[i].size(); j++)
			this->row_to_cols[this->col_to_rows[i][j]].push_back((unsigned)i);
}

void Sparse_matrix
::print(bool transpose, std::ostream& os) const
{
	if (transpose)
	{
		std::vector<unsigned> rows(this->n_rows, 0);

		for (auto& col : this->col_to_rows)
		{
			// set the ones
			for (auto& row : col)
				rows[row] = 1;

			for (auto& row : rows)
				os << row << " ";

			os << std::endl;

			// reset the ones
			for (auto& row : col)
				rows[row] = 0;
		}
	}
	else
	{
		std::vector<unsigned> columns(this->n_cols, 0);

		for (auto& row : this->row_to_cols)
		{
			// set the ones
			for (auto& col : row)
				columns[col] = 1;

			for (auto& col : columns)
				os << col << " ";

			os << std::endl;

			// reset the ones
			for (auto& col : row)
				columns[col] = 0;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Sparse_matrix& sm)
{
	sm.print(0, os);
	return os;
}