#include <Tools/Code/LDPC/QC_matrix/QC_matrix.hpp>
#include <string>
#include <sstream>
#include "Tools/Code/LDPC/AList/AList.hpp"

#include "Tools/Exception/exception.hpp"


using namespace aff3ct::tools;

std::vector<std::string> QC_matrix::split(const std::string &s)
{
	std::string buf;                 // have a buffer string
	std::stringstream ss(s);         // insert the string into a stream
	std::vector<std::string> tokens; // create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	return tokens;
}

void QC_matrix::getline(std::istream &file, std::string &line)
{
	if (file.eof() || file.fail() || file.bad())
		throw runtime_error(__FILE__, __LINE__, __func__, "Something went wrong when getting a new line.");

	while (std::getline(file, line))
		if (line[0] != '#' && !std::all_of(line.begin(),line.end(),isspace))
			break;
}

QC_matrix
::QC_matrix(const unsigned Nred, const unsigned Mred)
: Hred(Mred, mipp::vector<int16_t>(Nred, -1)),
  pctPattern(Nred, true),
  Z(0)
{
}

QC_matrix
::~QC_matrix()
{
}

Sparse_matrix QC_matrix::
expand_QC()
{
	unsigned Nred = (unsigned)this->Hred.size();
	unsigned Mred = (unsigned)this->Hred.front().size();

	unsigned N = Nred * this->Z;
	unsigned M = Mred * this->Z;

	Sparse_matrix H(N, M);

	for (unsigned i = 0; i < Nred; i++)
	{
		for (unsigned j = 0; j < Mred; j++)
		{
			auto value = this->Hred[i][j];

			unsigned idxLgn = i * this->Z;
			unsigned idxCol = j * this->Z;

			switch (value)
			{
				case -1:
					break;

				case 0:
					for (unsigned k = 0; k < this->Z; k++)
						H.add_connection(idxLgn + k, idxCol + k);
					break;

				default:
					for (unsigned k = 0; k < this->Z; k++)
						H.add_connection(idxLgn + k, (unsigned)(idxCol + (k + value) % this->Z));
					break;
			}
		}
	}
	return H.transpose();
}

std::vector<bool> QC_matrix::
get_pct_pattern() const
{
	return this->pctPattern;
}

QC_matrix QC_matrix
::load(std::istream &stream)
{
	std::string line;

	getline(stream, line);
	auto values = split(line);
	if (values.size() < 3)
	{
		std::stringstream message;
		message << "'values.size()' has to be greater than 2 ('values.size()' = " << values.size() << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	unsigned Nred = 0, Mred = 0, Z = 0;

	Nred = std::stoi(values[0]);
	Mred = std::stoi(values[1]);
	Z = std::stoi(values[2]);

	if (Nred <= 0 || Mred <= 0 || Z <= 0)
	{
		std::stringstream message;
		message << "'Nred', 'Mred' and 'Z' have to be greater than 0 ('Nred' = " << Nred
		        << ", 'Mred' = " << Mred << ", 'Z' = " << Z << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	QC_matrix matrix(Nred, Mred);
	matrix.Z = Z;

	for (unsigned i = 0; i < Mred; i++)
	{
		getline(stream, line);
		values = split(line);

		if (values.size() < Nred)
		{
			std::stringstream message;
			message << "'values.size()' has to be greater or equal to 'Nred' ('values.size()' = "
			        << values.size() << ", 'i' = " << i << ", 'Nred' = " << Nred << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (unsigned j = 0; j < Nred; j++)
		{
			auto col_value = (j < values.size()) ? std::stoi(values[j]) : -1;
			matrix.Hred[i][j] = col_value;
		}
	}

	//Try to get puncture pattern
	try
	{
		getline(stream, line);
		values = split(line);
		if (values.size() < Nred)
		{
			std::stringstream message;
			message << "'values.size()' has to be greater or equal to 'Nred' ('values.size()' = " << values.size() << ").";
			throw runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (unsigned j = 0; j < Nred; j++)
		{
			auto col_value = (j < values.size()) ? std::stoi(values[j]) : 1;
			matrix.pctPattern[j] = col_value;
		}
	}
	catch (std::exception const&)
	{
	}

	return matrix;
}

QC_matrix::QCFull_matrix QC_matrix
::invert_H2(const Sparse_matrix& _H)
{
	Sparse_matrix H;
	if (_H.get_n_rows() > _H.get_n_cols())
		H = _H.transpose();
	else
		H = _H;

	unsigned M = H.get_n_rows();
	unsigned N = H.get_n_cols();
	unsigned K = N - M;

	QC_matrix::QCFull_matrix MatriceCalcul;
	MatriceCalcul.clear();
	MatriceCalcul.resize(M, mipp::vector<int8_t>(2 * M, 0));

	//Copy H2 on left
	for (unsigned i = 0; i < M; i++)
		for (unsigned j = 0; j < H.get_cols_from_row(i).size(); j++)
			if (H.get_cols_from_row(i)[j] >= K)
				MatriceCalcul[i][H.get_cols_from_row(i)[j] - K] = 1;

	//Create identity on right
	for (unsigned i = 0; i < M; i++)
			MatriceCalcul[i][M + i] = 1;

	//Pivot de Gauss (Forward)
	for (unsigned indLgn = 0; indLgn < M; indLgn++)
	{
		if (MatriceCalcul[indLgn][indLgn] == 0)
		{
			unsigned indLgnSwap = 0;
			for (unsigned indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
			{
				if (MatriceCalcul[indLgn2][indLgn] != 0)
				{
					indLgnSwap = indLgn2;
					break;
				}
			}
			if (indLgnSwap == 0)
			{
				std::stringstream message;
				message << "Matrix H2 (H = [H1 H2]) is not invertible";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
			std::swap(MatriceCalcul[indLgn], MatriceCalcul[indLgnSwap]);
		}

		//XOR des lignes
		for (unsigned indLgn2 = indLgn + 1; indLgn2 < M; indLgn2++)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (unsigned)(2 * M / mipp::nElReg<int8_t>());
				for (unsigned i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<int8_t>(&MatriceCalcul[indLgn][i * mipp::nElReg<int8_t>()]);
					const auto rLgn2 = mipp::Reg<int8_t>(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
				}
				for (unsigned i = loop_size1 * mipp::nElReg<int8_t>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}

	//Pivot de Gauss (Backward)
	for (unsigned indLgn = M - 1; indLgn > 0; indLgn--)
	{
		//XOR des lignes
		for (unsigned indLgn2 = indLgn - 1; indLgn2 > 0; indLgn2--)
		{
			if (MatriceCalcul[indLgn2][indLgn] != 0)
			{
				const auto loop_size1 = (unsigned)(2 * M / mipp::nElReg<int8_t>());
				for (unsigned i = 0; i < loop_size1; i++)
				{
					const auto rLgn  = mipp::Reg<int8_t>(&MatriceCalcul[indLgn][i * mipp::nElReg<int8_t>()]);
					const auto rLgn2 = mipp::Reg<int8_t>(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
					auto rLgn3       = rLgn2 ^ rLgn;
					rLgn3.store(&MatriceCalcul[indLgn2][i * mipp::nElReg<int8_t>()]);
				}
				for (unsigned i = loop_size1 * mipp::nElReg<int8_t>(); i < 2 * M; i++)
					MatriceCalcul[indLgn2][i] = MatriceCalcul[indLgn2][i] ^ MatriceCalcul[indLgn][i];
			}
		}
	}

	QC_matrix::QCFull_matrix invH2;
	invH2.clear();
	invH2.resize(M, mipp::vector<int8_t>(M, 0));

	for (unsigned i = 0; i < M; i++)
		for (unsigned j = 0; j < M; j++)
			invH2[i][j] = MatriceCalcul[i][M + j];

	MatriceCalcul.clear();

	return invH2;
}
