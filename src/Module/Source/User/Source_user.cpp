#include <fstream>
#include <stdexcept>

#include "Source_user.hpp"

using namespace aff3ct::module;

template <typename B>
Source_user<B>
::Source_user(const int K, const std::string filename, const int n_frames, const std::string name)
: Source<B>(K, n_frames, name), source(), src_counter(0)
{
	if (filename.empty())
		throw std::invalid_argument("aff3ct::module::Source_user: path to the file should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_src = 0, src_size = 0;

		file >> n_src;
		file >> src_size;

		if (n_src <= 0 || src_size <= 0)
			throw std::runtime_error("aff3ct::module::Source_user: \"n_src\", and \"src_size\" have to be "
			                         "greater than 0.");

		this->source.resize(n_src);
		for (auto i = 0; i < n_src; i++)
			this->source[i].resize(src_size);

		if (src_size == this->K)
		{
			for (auto i = 0; i < n_src; i++)
				for (auto j = 0; j < src_size; j++)
				{
					B bit;
					file >> bit;

					this->source[i][j] = bit != 0;
				}
		}
		else
		{
			file.close();

			throw std::runtime_error("aff3ct::module::Source_user: the size is wrong (read: " +
			                         std::to_string(src_size) + ", expected: " + std::to_string(this->K) + ").");
		}

		file.close();
	}
	else
	{
		throw std::invalid_argument("aff3ct::module::Source_user: can't open \"" + filename + "\" file.");
	}
}

template <typename B>
Source_user<B>
::~Source_user()
{
}

template <typename B>
void Source_user<B>
::_generate(mipp::vector<B>& U_K)
{
	for (auto f = 0; f < this->n_frames; f++)
	{
		std::copy(this->source[this->src_counter].begin(),
		          this->source[this->src_counter].end  (),
		          U_K.begin() + f * this->K);

		this->src_counter = (this->src_counter +1) % (int)this->source.size();
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Source_user<B_8>;
template class aff3ct::module::Source_user<B_16>;
template class aff3ct::module::Source_user<B_32>;
template class aff3ct::module::Source_user<B_64>;
#else
template class aff3ct::module::Source_user<B>;
#endif
// ==================================================================================== explicit template instantiation
