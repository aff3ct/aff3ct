#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <string>
#include <mipp.h>

#include "Tools/Code/LDPC/AList/AList.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Tools/Code/LDPC/QC/QC.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Sparse_matrix QC
::read(std::istream &stream)
{
	try
	{
		return QC::_read(stream);
	}
	catch (std::exception const&)
	{
		std::stringstream message;
		message << "The given stream does not refer to a QC format file.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

Sparse_matrix QC
::_read(std::istream &stream)
{
	// ----------------------------------------------------------------------------------------- read matrix from file
	std::string line;

	getline(stream, line);
	auto values = split(line);
	if (values.size() < 3)
	{
		std::stringstream message;
		message << "'values.size()' has to be greater than 2 ('values.size()' = " << values.size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	unsigned N_red = 0, M_red = 0, Z = 0;

	N_red = std::stoi(values[0]);
	M_red = std::stoi(values[1]);
	Z = std::stoi(values[2]);

	if (N_red == 0 || M_red == 0 || Z == 0)
	{
		std::stringstream message;
		message << "'N_red', 'M_red' and 'Z' have to be greater than 0 ('N_red' = " << N_red
		        << ", 'M_red' = " << M_red << ", 'Z' = " << Z << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<mipp::vector<int16_t>> H_red(M_red, mipp::vector<int16_t>(N_red, -1));

	for (unsigned i = 0; i < M_red; i++)
	{
		getline(stream, line);
		values = split(line);

		if (values.size() < N_red)
		{
			std::stringstream message;
			message << "'values.size()' has to be greater or equal to 'N_red' ('values.size()' = "
			        << values.size() << ", 'i' = " << i << ", 'N_red' = " << N_red << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (unsigned j = 0; j < N_red; j++)
		{
			auto col_value = (j < values.size()) ? std::stoi(values[j]) : -1;
			H_red[i][j] = col_value;
		}
	}

	// ---------------------------------------------------------------------------- expand QC format into sparse format
	unsigned N = M_red * Z;
	unsigned M = N_red * Z;

	Sparse_matrix H(N, M);

	for (unsigned i = 0; i < M_red; i++)
	{
		for (unsigned j = 0; j < N_red; j++)
		{
			auto value = H_red[i][j];

			unsigned idxLgn = i * Z;
			unsigned idxCol = j * Z;

			switch (value)
			{
				case -1:
					break;

				case 0:
					for (unsigned k = 0; k < Z; k++)
						H.add_connection(idxLgn + k, idxCol + k);
					break;

				default:
					for (unsigned k = 0; k < Z; k++)
						H.add_connection(idxLgn + k, (unsigned)(idxCol + (k + value) % Z));
					break;
			}
		}
	}

	return H.transpose();
}

std::vector<bool> QC
::read_pct_pattern(std::istream &stream, int N_red)
{
	std::string line;
	std::vector<bool> pattern;

	// try to get puncture pattern
	try
	{
		getline(stream, line);
		auto values = split(line);
		if ((int)values.size() < N_red && N_red != -1)
		{
			std::stringstream message;
			message << "'values.size()' has to be greater or equal to 'N_red' ('values.size()' = "
			        << values.size() << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
		else
			N_red = (unsigned)values.size();

		for (auto j = 0; j < N_red; j++)
		{
			auto col_value = (j < (int)values.size()) ? std::stoi(values[j]) : 1;
			pattern.push_back(col_value);
		}
	}
	catch (std::exception const&)
	{
		std::stringstream message;
		message << "Something went wrong when trying to read the LDPC puncturing pattern.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return pattern;
}

void QC
::write(const Sparse_matrix &matrix, std::ostream &stream)
{
	AList::write(matrix, stream);
}

void QC
::read_matrix_size(std::istream &stream, int& H, int& N)
{
	std::string line;

	tools::getline(stream, line);
	auto values = split(line);
	if (values.size() < 3)
	{
		std::stringstream message;
		message << "'values.size()' has to be greater than 2 ('values.size()' = " << values.size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	unsigned N_red = 0, M_red = 0, Z = 0;

	N_red = std::stoi(values[0]);
	M_red = std::stoi(values[1]);
	Z = std::stoi(values[2]);

	if (N_red == 0 || M_red == 0 || Z == 0)
	{
		std::stringstream message;
		message << "'N_red', 'M_red' and 'Z' have to be greater than 0 ('N_red' = " << N_red
		        << ", 'M_red' = " << M_red << ", 'Z' = " << Z << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	N = N_red * Z;
	H = M_red * Z;
}