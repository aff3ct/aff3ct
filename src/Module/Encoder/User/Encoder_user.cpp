#include <ios>
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Module/Encoder/User/Encoder_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

std::vector<uint32_t> read_info_bits_pos(std::istream &stream)
{
	std::string line;

	// look for the position in the file where the info bits begin
	while (std::getline(stream, line))
		if (line == "# Positions of the information bits in the codewords:" || stream.eof() || stream.fail() || stream.bad())
			break;

	getline(stream, line);
	auto values = tools::split(line);
	if (values.size() != 1)
	{
		std::stringstream message;
		message << "'values.size()' has to be equal to 1 ('values.size()' = " << values.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	const uint32_t size = std::stoi(values[0]);

	getline(stream, line);
	values = tools::split(line);
	if (values.size() != size)
	{
		std::stringstream message;
		message << "'values.size()' has to be equal to 'size' ('values.size()' = " << values.size()
		        << ", 'size' = " << size << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<uint32_t> info_bits_pos;
	for (auto v : values)
	{
		const uint32_t pos = std::stoi(v);

		if (std::find(info_bits_pos.begin(), info_bits_pos.end(), pos) != info_bits_pos.end())
		{
			std::stringstream message;
			message << "'pos' already exists in the 'info_bits_pos' vector ('pos' = " << pos << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		info_bits_pos.push_back(pos);
	}

	return info_bits_pos;
}

template <typename B>
Encoder_user<B>
::Encoder_user(const int K, const int N, const std::string &filename, const int start_idx)
: Encoder<B>(K, N), codewords(), cw_counter(start_idx)
{
	const std::string name = "Encoder_user";
	this->set_name(name);

	if (filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_cw = 0, src_size = 0, cw_size = 0;

		file >> n_cw;
		file >> cw_size;
		file >> src_size;

		if (n_cw <= 0 || src_size <= 0 || cw_size <= 0)
		{
			std::stringstream message;
			message << "'n_cw', 'src_size' and 'cw_size' have to be greater than 0 ('n_cw' = " << n_cw
			        << ", 'src_size' = " << src_size << ", 'cw_size' = " << cw_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (cw_size < src_size)
		{
			std::stringstream message;
			message << "'cw_size' has to be equal or greater than 'src_size' ('cw_size' = " << cw_size
			        << ", 'src_size' = " << src_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->codewords.resize(n_cw);
		for (auto i = 0; i < n_cw; i++)
			this->codewords[i].resize(cw_size);

		if ((src_size == this->K) && (cw_size == this->N))
		{
			for (auto i = 0; i < n_cw; i++)
				for (auto j = 0; j < cw_size; j++)
				{
					int symbol;
					file >> symbol;
					this->codewords[i][j] = (B)symbol;
				}
		}
		else
		{
			std::stringstream message;
			message << "The number of information bits or the codeword size is wrong "
			        << "(read: {" << src_size << "," << cw_size << "}, "
			        << "expected: {" << this->K << "," << this->N << "}).";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		try
		{
			this->info_bits_pos = read_info_bits_pos(file);
		}
		catch (std::exception const&)
		{
			// information bits positions are not in the matrix file
		}

		if ((int)this->info_bits_pos.size() != this->K)
		{
			std::stringstream message;
			message << "'this->info_bits_pos.size()' has to be equal to 'this->K' ('this->info_bits_pos.size()' = "
			        << this->info_bits_pos.size() << ", 'this->K' = " << this->K << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}
	else
	{
		std::stringstream message;
		message << "Can't open '" + filename + "' file.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	cw_counter %= (int)codewords.size();
}

template <typename B>
Encoder_user<B>* Encoder_user<B>
::clone() const
{
	auto m = new Encoder_user(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_user<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	std::copy(this->codewords[this->cw_counter].begin(),
	          this->codewords[this->cw_counter].end  (),
	          X_N);

	this->cw_counter = (this->cw_counter +1) % (int)this->codewords.size();
}

template <typename B>
bool Encoder_user<B>
::is_sys() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_user<B_8>;
template class aff3ct::module::Encoder_user<B_16>;
template class aff3ct::module::Encoder_user<B_32>;
template class aff3ct::module::Encoder_user<B_64>;
#else
template class aff3ct::module::Encoder_user<B>;
#endif
// ==================================================================================== explicit template instantiation
