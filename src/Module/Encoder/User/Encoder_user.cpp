#include <cassert>
#include <fstream>

#include "Tools/Display/bash_tools.h"

#include "Encoder_user.hpp"
using namespace aff3ct;

template <typename B>
Encoder_user<B>
::Encoder_user(const int K, const int N, const std::string filename, const int n_frames, const std::string name)
: Encoder<B>(K, N, n_frames, name), codewords(), cw_counter(0)
{
	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_cw = 0, src_size = 0, cw_size = 0;

		file >> n_cw;
		file >> src_size;
		file >> cw_size;

		assert(n_cw > 0 && src_size > 0 && cw_size > 0);
		assert(cw_size >= src_size);

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
			std::cerr << bold_red("(EE) The number of information bits or the codeword size is wrong (read: {")
			          << bold_red(std::to_string(src_size)) << bold_red(",")
			          << bold_red(std::to_string(cw_size))
			          << bold_red("}, expected: {") << bold_red(std::to_string(this->K)) << bold_red(",")
			          << bold_red(std::to_string(this->N))
			          << bold_red("}), exiting.") << std::endl;
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
Encoder_user<B>
::~Encoder_user()
{
}

template <typename B>
void Encoder_user<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() == (unsigned)(this->K * this->n_frames));
	assert(X_N.size() == (unsigned)(this->N * this->n_frames));

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
template class Encoder_user<B_8>;
template class Encoder_user<B_16>;
template class Encoder_user<B_32>;
template class Encoder_user<B_64>;
#else
template class Encoder_user<B>;
#endif
// ==================================================================================== explicit template instantiation
