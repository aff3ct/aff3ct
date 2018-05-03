#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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
	auto itr = std::find(this->row_to_cols[row_index].begin(), this->row_to_cols[row_index].end(), col_index);
	if (itr != this->row_to_cols[row_index].end());
	{
		this->row_to_cols[row_index].erase(itr);

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
	auto itc = std::find(this->col_to_rows[col_index].begin(), this->col_to_rows[col_index].end(), row_index);
	if (itc != this->col_to_rows[col_index].end());
	{
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

	if ((itr == this->row_to_cols[row_index].end()) != (itc == this->col_to_rows[col_index].end()))
	{
		std::stringstream message;
		message << "The connection has been found only in one of the two vectors 'row_to_cols' and 'col_to_rows' "
		        << "('row_index' = " << row_index << "'col_index' = " << col_index << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->n_connections--;
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
			this->row_to_cols[this->col_to_rows[i][j]].push_back(i);
}
