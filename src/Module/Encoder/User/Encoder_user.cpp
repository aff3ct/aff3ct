#include <fstream>
#include <stdexcept>

#include "Encoder_user.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_user<B>
::Encoder_user(const int K, const int N, const std::string filename, const int n_frames, const std::string name)
: Encoder<B>(K, N, n_frames, name), codewords(), cw_counter(0)
{
	if (filename.empty())
		throw std::invalid_argument("aff3ct::module::Encoder_user: path to the file should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_cw = 0, src_size = 0, cw_size = 0;

		file >> n_cw;
		file >> src_size;
		file >> cw_size;

		if (n_cw <= 0 || src_size <= 0 || cw_size <= 0)
			throw std::runtime_error("aff3ct::module::Encoder_user: \"n_cw\", \"src_size\" and \"cw_size\" have to be "
			                         "greater than 0.");

		if (cw_size < src_size)
			throw std::runtime_error("aff3ct::module::Encoder_user: \"cw_size\" has to be equal or greater than "
			                         "\"src_size\".");

		this->codewords.resize(n_cw);
		for (auto i = 0; i < n_cw; i++)
			this->codewords[i].resize(cw_size);

		if ((src_size == this->K) && (cw_size == this->N))
		{
			for (auto i = 0; i < n_cw; i++)
				for (auto j = 0; j < cw_size; j++)
				{
					B symbol;
					file >> symbol;
					this->codewords[i][j] = symbol;
				}
		}
		else
		{
			file.close();

			throw std::runtime_error("aff3ct::module::Encoder_user: the number of information bits or the codeword "
			                         "size is wrong (read: {" + std::to_string(src_size) + "," +
			                         std::to_string(cw_size) + "}, expected: {" + std::to_string(this->K) + "," +
			                         std::to_string(this->N) + "}).");
		}

		file.close();
	}
	else
	{
		throw std::invalid_argument("aff3ct::module::Encoder_user: can't open \"" + filename + "\" file.");
	}
}

template <typename B>
Encoder_user<B>
::~Encoder_user()
{
}

template <typename B>
void Encoder_user<B>
::_encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	for (auto f = 0; f < this->n_frames; f++)
	{
		std::copy(this->codewords[this->cw_counter].begin(),
		          this->codewords[this->cw_counter].end  (),
		          X_N.begin() + f * this->N);

		this->cw_counter = (this->cw_counter +1) % (int)this->codewords.size();
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_user<B_8>;
template class aff3ct::module::Encoder_user<B_16>;
template class aff3ct::module::Encoder_user<B_32>;
template class aff3ct::module::Encoder_user<B_64>;
#else
template class aff3ct::module::Encoder_user<B>;
#endif
// ==================================================================================== explicit template instantiation
