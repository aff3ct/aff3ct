#include <string>
#include <sstream>
#include <vector>

#include "Tools/Exception/exception.hpp"

#include "Sparse_matrix.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Sparse_matrix
::Sparse_matrix(const size_t n_rows, const size_t n_cols)
: Matrix(n_rows, n_cols),
  row_to_cols(n_rows),
  col_to_rows(n_cols)
{
}

bool Sparse_matrix
::at(const size_t row_index, const size_t col_index) const
{
	auto it = std::find(this->row_to_cols[row_index].begin(), this->row_to_cols[row_index].end(), col_index);
	return (it != this->row_to_cols[row_index].end());
}

void Sparse_matrix
::add_connection(const size_t row_index, const size_t col_index)
{
	check_indexes(row_index, col_index);

	if (at(row_index, col_index))
	{
		std::stringstream message;
		message << "('row_index';'col_index') connection already exists ('row_index' = " << row_index
		        << ", 'col_index' = " << col_index << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->row_to_cols[row_index].push_back(col_index);
	this->col_to_rows[col_index].push_back(row_index);

	this->rows_max_degree = std::max(get_rows_max_degree(), row_to_cols[row_index].size());
	this->cols_max_degree = std::max(get_cols_max_degree(), col_to_rows[col_index].size());

	this->n_connections++;
}

void Sparse_matrix
::rm_connection(const size_t row_index, const size_t col_index)
{
	check_indexes(row_index, col_index);

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

void Sparse_matrix
::self_resize(const size_t n_rows, const size_t n_cols, Origin o)
{
	// const auto min_r = std::min(n_rows, get_n_rows());

	// switch(o)
	// {
	// 	case Origin::TOP_LEFT:
	// 	{
	// 		values.resize(n_rows, std::vector<T>(n_cols, 0));
	// 		for (size_t r = 0; r < min_r; r++)
	// 			values[r].resize(n_cols, 0);
	// 	}
	// 	break;
	// 	case Origin::TOP_RIGHT:
	// 	{
	// 		values.resize(n_rows, std::vector<T>(n_cols, 0));
	// 		if (n_cols < get_n_cols())
	// 		{
	// 			auto n_erase = get_n_cols() - n_cols;
	// 			for (size_t r = 0; r < n_rows; r++)
	// 				values[r].erase(values[r].begin(), values[r].begin() + n_erase);
	// 		}
	// 		else
	// 		{
	// 			auto n_insert = n_cols - get_n_cols();
	// 			for (size_t r = 0; r < min_r; r++)
	// 				values[r].insert(values[r].begin(), n_insert, 0);
	// 		}
	// 	}
	// 	break;
	// 	case Origin::BOTTOM_LEFT:
	// 	{
	// 		if (n_rows < get_n_rows())
	// 		{
	// 			auto n_erase = get_n_rows() - n_rows;
	// 			values.erase(values.begin(), values.begin() + n_erase);

	// 			for (size_t r = 0; r < min_r; r++)
	// 				values[r].resize(n_cols, 0);
	// 		}
	// 		else
	// 		{
	// 			auto n_insert = n_rows - get_n_rows();
	// 			values.insert(values.begin(), n_insert, std::vector<T>(n_cols, 0));

	// 			for (size_t r = n_insert; r < n_rows; r++)
	// 				values[r].resize(n_cols, 0);
	// 		}
	// 	}
	// 	break;
	// 	case Origin::BOTTOM_RIGHT:
	// 	{
	// 		if (n_rows < get_n_rows())
	// 		{
	// 			auto n_erase = get_n_rows() - n_rows;
	// 			values.erase(values.begin(), values.begin() + n_erase);

	// 			if (n_cols < get_n_cols())
	// 			{
	// 				auto n_erase = get_n_cols() - n_cols;
	// 				for (size_t r = 0; r < n_rows; r++)
	// 					values[r].erase(values[r].begin(), values[r].begin() + n_erase);
	// 			}
	// 			else
	// 			{
	// 				auto n_insert = n_cols - get_n_cols();
	// 				for (size_t r = 0; r < min_r; r++)
	// 					values[r].insert(values[r].begin(), n_insert, 0);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			auto n_insert = n_rows - get_n_rows();
	// 			values.insert(values.begin(), n_insert, std::vector<T>(n_cols, 0));

	// 			if (n_cols < get_n_cols())
	// 			{
	// 				auto n_erase = get_n_cols() - n_cols;
	// 				for (size_t r = n_insert; r < n_rows; r++)
	// 					values[r].erase(values[r].begin(), values[r].begin() + n_erase);
	// 			}
	// 			else
	// 			{
	// 				auto n_insert = n_cols - get_n_cols();
	// 				for (size_t r = n_insert; r < min_r; r++)
	// 					values[r].insert(values[r].begin(), n_insert, 0);
	// 			}
	// 		}
	// 	}
	// 	break;
	// }

	// Matrix::self_resize(n_rows, n_cols);

	// parse_connections();
}

Sparse_matrix Sparse_matrix
::resize(const size_t n_rows, const size_t n_cols, Origin o) const
{
	Sparse_matrix resized(*this);

	resized.self_resize(n_rows, n_cols, o);

	return resized;
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
	Matrix::self_transpose();

	std::swap(row_to_cols, col_to_rows);
}

Sparse_matrix Sparse_matrix
::turn(Way w) const
{
	Sparse_matrix turned(*this);

	turned.self_turn(w);

	return turned;
}

void Sparse_matrix
::sort_cols_per_density(Sort order)
{
	switch(order)
	{
		case Sort::ASCENDING:
			std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
			          [](const std::vector<size_t> &i1, const  std::vector<size_t> &i2) { return i1.size() < i2.size(); });
		break;
		case Sort::DESCENDING:
			std::sort(this->col_to_rows.begin(), this->col_to_rows.end(),
		          [](const  std::vector<size_t> &i1, const std::vector<size_t> &i2) { return i1.size() > i2.size(); });
		break;
	}

	for (auto &r : this->row_to_cols)
		r.clear();
	for (size_t i = 0; i < this->col_to_rows.size(); i++)
		for (size_t j = 0; j < this->col_to_rows[i].size(); j++)
			this->row_to_cols[this->col_to_rows[i][j]].push_back(i);
}

void Sparse_matrix
::print(bool transpose, std::ostream& os) const
{
	if (transpose)
	{
		std::vector<unsigned> rows(get_n_rows(), 0);

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
		std::vector<unsigned> columns(get_n_cols(), 0);

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

Sparse_matrix Sparse_matrix
::identity(const size_t n_rows, const size_t n_cols)
{
	Sparse_matrix mat(n_rows, n_cols);
	auto shortest_side = std::min(n_rows, n_cols);

	for (size_t i = 0; i < shortest_side; i++)
		mat.add_connection(i,i);

	return mat;
}

Sparse_matrix Sparse_matrix
::zero(const size_t n_rows, const size_t n_cols)
{
	return Sparse_matrix(n_rows, n_cols);
}