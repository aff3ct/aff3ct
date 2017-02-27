#include <fstream>

#include "Tools/Display/bash_tools.h"

#include "Source_user.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B>
Source_user<B>
::Source_user(const int K, const std::string filename, const int n_frames, const std::string name)
: Source<B>(K, n_frames, name), source(), src_counter(0)
{
	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_src = 0, src_size = 0;

		file >> n_src;
		file >> src_size;

		assert(n_src > 0 && src_size > 0);

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

					assert(bit == 0 || bit == 1);
					this->source[i][j] = bit;
				}
		}
		else
		{
			std::cerr << bold_red("(EE) The source size is wrong (read: ") << bold_red(std::to_string(src_size))
			          << bold_red(", expected: ") << bold_red(std::to_string(this->K))
			          << bold_red("), exiting.") << std::endl;
			file.close();
			std::exit(-1);
		}

		file.close();
	}
	else
	{
		std::cerr << bold_red("(EE) Can't open \"") << bold_red(filename) << bold_red("\" file, exiting.")
		          << std::endl;
		std::exit(-1);
	}
}

template <typename B>
Source_user<B>
::~Source_user()
{
}

template <typename B>
void Source_user<B>
::generate(mipp::vector<B>& U_K)
{
	assert((int)U_K.size() == this->K * this->n_frames);

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
