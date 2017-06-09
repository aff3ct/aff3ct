#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "AList.hpp"

using namespace aff3ct::tools;

std::vector<std::string> split(const std::string &s)
{
	std::string buf;                 // have a buffer string
	std::stringstream ss(s);         // insert the string into a stream
	std::vector<std::string> tokens; // create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

void getline(std::istream &file, std::string &line)
{
	if (file.eof() || file.fail() || file.bad())
		throw std::runtime_error("aff3ct::tools::getline: something went wrong when getting a new line.");

	while (std::getline(file, line))
		if (line[0] != '#' && !std::all_of(line.begin(),line.end(),isspace))
			break;
}

Sparse_matrix AList
::read(std::istream &stream)
{
	// save the init pos of the stream
	auto init_pos = stream.tellg();

	try
	{
		return AList::read_format1(stream);
	}
	catch (std::exception const&)
	{
		try
		{
			stream.clear();
			stream.seekg(init_pos);
			return AList::read_format2(stream);
		}
		catch (std::exception const&)
		{
			stream.clear();
			stream.seekg(init_pos);
			return AList::read_format3(stream);
		}
	}
}

void AList
::write(const Sparse_matrix &matrix, std::ostream &stream)
{
	stream << matrix.get_n_rows()          << " " << matrix.get_n_cols()          << std::endl << std::endl;
	stream << matrix.get_rows_max_degree() << " " << matrix.get_cols_max_degree() << std::endl << std::endl;

	for (const auto &r : matrix.get_row_to_cols())
		stream << r.size() << " ";
	stream << std::endl << std::endl;

	for (const auto &c : matrix.get_col_to_rows())
		stream << c.size() << " ";
	stream << std::endl << std::endl;

	for (const auto &r : matrix.get_row_to_cols())
	{
		unsigned i;
		for (i = 0; i < r.size(); i++)
			stream << (r[i] +1) << " ";
		for (; i < matrix.get_rows_max_degree(); i++)
			stream << 0 << " ";
		stream << std::endl;
	}
	stream << std::endl;

	for (const auto &c : matrix.get_col_to_rows())
	{
		unsigned i;
		for (i = 0; i < c.size(); i++)
			stream << (c[i] +1) << " ";
		for (; i < matrix.get_cols_max_degree(); i++)
			stream << 0 << " ";
		stream << std::endl;
	}
}

std::vector<unsigned> AList
::read_info_bits_pos(std::istream &stream, const int K)
{
	std::string line;

	getline(stream, line);
	auto values = split(line);
	if (values.size() != 1)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be equal to 1.");

	const unsigned size = std::stoi(values[0]);

	if (K != -1 && size != (unsigned)K)
		throw std::runtime_error("aff3ct::tools::AList: \"size\" has to be equal to \"K\".");

	getline(stream, line);
	values = split(line);
	if (values.size() != size)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be equal to \"size\".");

	std::vector<unsigned> info_bits_pos;
	for (auto v : values)
	{
		const unsigned pos = std::stoi(v);

		if (std::find(info_bits_pos.begin(), info_bits_pos.end(), pos) != info_bits_pos.end())
			throw std::runtime_error("aff3ct::tools::AList: \"pos\" already exists in the \"info_bits_pos\" vector.");

		if (K != -1 && pos >= (unsigned)K)
			throw std::runtime_error("aff3ct::tools::AList: \"pos\" has to be smaller than \"K\".");

		info_bits_pos.push_back(pos);
	}

	return info_bits_pos;
}

void AList
::write_info_bits_pos(const std::vector<unsigned> &info_bits_pos, std::ostream &stream)
{
	stream << "# Position of the information bits in the codeword:" << std::endl;
	stream << info_bits_pos.size() << std::endl;
	for (auto pos : info_bits_pos)
		stream << pos << " ";
	stream << std::endl;
}

// perfect AList format
Sparse_matrix AList
::read_format1(std::istream &stream)
{
	unsigned n_rows = 0, n_cols = 0, rows_max_degree = 0, cols_max_degree = 0;

	stream >> n_rows;
	stream >> n_cols;
	stream >> rows_max_degree;
	stream >> cols_max_degree;

	if (n_rows > 0 && n_cols > 0 && rows_max_degree > 0 && cols_max_degree > 0)
	{
		Sparse_matrix matrix(n_rows, n_cols);

		std::vector<unsigned> rows_degree(n_rows);
		for (unsigned i = 0; i < n_rows; i++)
		{
			unsigned n_connections = 0;
			stream >> n_connections;

			if (n_connections <= 0 || n_connections > rows_max_degree)
				throw std::runtime_error("aff3ct::tools::AList: \"n_connections\" has to be greater than 0 and "
				                         "smaller or equal to \"rows_max_degree\".");

			rows_degree[i] = n_connections;
		}

		std::vector<unsigned> cols_degree(n_cols);
		for (unsigned i = 0; i < n_cols; i++)
		{
			unsigned n_connections = 0;
			stream >> n_connections;

			if (n_connections <= 0 || n_connections > cols_max_degree)
				throw std::runtime_error("aff3ct::tools::AList: \"n_connections\" has to be greater than 0 and "
				                         "smaller or equal to \"cols_max_degree\".");

			cols_degree[i] = n_connections;
		}

		for (unsigned i = 0; i < n_rows; i++)
		{
			for (unsigned j = 0; j < rows_max_degree; j++)
			{
				unsigned col_index = 0;

				stream >> col_index;

				if ((col_index >  0 && j <  rows_degree[i]) ||
					(col_index <= 0 && j >= rows_degree[i]))
				{
					if (col_index)
						matrix.add_connection(i, col_index -1);
				}
				else
					throw std::runtime_error("aff3ct::tools::AList: \"col_index\" is wrong.");
			}
		}

		for (unsigned i = 0; i < n_cols; i++)
		{
			for (unsigned j = 0; j < cols_max_degree; j++)
			{
				unsigned row_index = 0;
				stream >> row_index;

				if ((row_index >  0 && j <  cols_degree[i]) ||
					(row_index <= 0 && j >= cols_degree[i]))
				{
					if (row_index)
					{
						try
						{
							matrix.add_connection(row_index -1, i);
							throw std::runtime_error("aff3ct::tools::AList: the input AList file is not consistent.");
						}
						catch (std::exception const&)
						{
							// everything is normal if the code passes through the catch
						}
					}
				}
				else
					throw std::runtime_error("aff3ct::tools::AList: \"row_index\" is wrong.");
			}
		}

		return matrix;
	}
	else
		throw std::runtime_error("aff3ct::tools::AList: \"rows\", \"cols\", \"rows_max_degree\" and "
		                         "\"cols_max_degree\" have to be greater than 0.");
}

// format with comments or/and no padding with 0
Sparse_matrix AList
::read_format2(std::istream &stream)
{
	std::string line;

	getline(stream, line);
	auto values = split(line);
	if (values.size() < 2)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater than 1.");

	unsigned n_rows = 0, n_cols = 0, rows_max_degree = 0, cols_max_degree = 0;

	n_rows = std::stoi(values[0]);
	n_cols = std::stoi(values[1]);

	if (n_rows <= 0 || n_cols <= 0)
		throw std::runtime_error("aff3ct::tools::AList: \"rows\" and \"cols\" have to be greater than 0.");

	Sparse_matrix matrix(n_rows, n_cols);

	getline(stream, line);
	values = split(line);
	if (values.size() < 2)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater than 1.");

	rows_max_degree = std::stoi(values[0]);
	cols_max_degree = std::stoi(values[1]);

	if (rows_max_degree <= 0 || cols_max_degree <= 0)
		throw std::runtime_error("aff3ct::tools::AList: \"rows_max_degree\" and \"cols_max_degree\" have to be "
		                         "greater than 0.");

	getline(stream, line);
	values = split(line);
	if (values.size() < n_rows)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal to \"rows\".");

	std::vector<unsigned> rows_degree(n_rows);
	for (unsigned i = 0; i < n_rows; i++)
	{
		unsigned n_connections = std::stoi(values[i]);
		if (n_connections > 0 && n_connections <= rows_max_degree)
			rows_degree[i] = n_connections;
		else
			throw std::runtime_error("aff3ct::tools::AList: \"n_connections\" has to be greater than 0 and "
			                         "smaller than \"rows_max_degree\".");
	}

	getline(stream, line);
	values = split(line);
	if (values.size() < n_cols)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal to \"cols\".");

	std::vector<unsigned> cols_degree(n_rows);
	for (unsigned i = 0; i < n_cols; i++)
	{
		unsigned n_connections = std::stoi(values[i]);
		if (n_connections > 0 && n_connections <= cols_max_degree)
			cols_degree[i] = n_connections;
		else
			throw std::runtime_error("aff3ct::tools::AList: \"n_connections\" has to be greater than 0 and "
			                         "smaller than \"cols_max_degree\".");
	}

	for (unsigned i = 0; i < n_rows; i++)
	{
		getline(stream, line);
		values = split(line);

		if (values.size() < rows_degree[i])
			throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal "
			                         "to \"rows_degree[i]\".");

		for (unsigned j = 0; j < rows_max_degree; j++)
		{
			auto col_index = (j < values.size()) ? std::stoi(values[j]) : 0;
			if ((col_index >  0 && j <  rows_degree[i]) ||
				(col_index <= 0 && j >= rows_degree[i]))
			{
				if (col_index)
					matrix.add_connection(i, col_index -1);
			}
			else
				throw std::runtime_error("aff3ct::tools::AList: \"col_index\" is wrong.");
		}
	}

	for (unsigned i = 0; i < n_cols; i++)
	{
		getline(stream, line);
		values = split(line);

		if (values.size() < cols_degree[i])
			throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal "
			                         "to \"cols_degree[i]\".");

		for (unsigned j = 0; j < cols_max_degree; j++)
		{
			auto row_index = (j < values.size()) ? std::stoi(values[j]) : 0;
			if ((row_index >  0 && j <  cols_degree[i]) ||
				(row_index <= 0 && j >= cols_degree[i]))
			{
				if (row_index)
				{
					try
					{
						matrix.add_connection(row_index -1, i);
						throw std::runtime_error("aff3ct::tools::AList: the input AList file is not consistent.");
					}
					catch (std::exception const&)
					{
						// everything is normal if the code passes through the catch
					}
				}
			}
			else
				throw std::runtime_error("aff3ct::tools::AList: \"row_index\" is wrong.");
		}
	}

	return matrix;
}

// format ANR NAND
Sparse_matrix AList
::read_format3(std::istream &stream)
{
	std::string line;

	getline(stream, line);
	auto values = split(line);
	if (values.size() < 2)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater than 1.");

	unsigned n_rows = 0, n_cols = 0;

	n_cols = std::stoi(values[0]);
	n_rows = std::stoi(values[1]);

	if (n_rows <= 0 || n_cols <= 0)
		throw std::runtime_error("aff3ct::tools::AList: \"n_rows\" and \"n_cols\" have to be greater than 0.");

	Sparse_matrix matrix(n_rows, n_cols);

	getline(stream, line);
	values = split(line);
	if (values.size() < n_cols)
		throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal to \"n_cols\".");

	std::vector<unsigned> cols_degree(n_cols);
	for (unsigned i = 0; i < n_cols; i++)
	{
		unsigned n_connections = std::stoi(values[i]);
		if (n_connections > 0 && n_connections <= n_rows)
			cols_degree[i] = n_connections;
		else
			throw std::runtime_error("aff3ct::tools::AList: \"n_connections\" has to be greater than 0 and "
			                         "smaller than \"n_rows\".");
	}

	for (unsigned i = 0; i < n_cols; i++)
	{
		getline(stream, line);
		values = split(line);

		if (values.size() < cols_degree[i])
			throw std::runtime_error("aff3ct::tools::AList: \"values.size()\" has to be greater or equal "
			                         "to \"cols_degree[i]\".");

		for (unsigned j = 0; j < cols_degree[i]; j++)
		{
			unsigned row_index = std::stoi(values[j]);
			matrix.add_connection(row_index, i);
		}
	}

	return matrix;
}
