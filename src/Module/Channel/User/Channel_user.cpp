
#include <fstream>
#include <cassert>

#include "Tools/Display/bash_tools.h"
#include "Channel_user.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R>
Channel_user<R>
::Channel_user(const int N, const std::string filename, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name), noise(), noise_counter(0)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (file.is_open())
	{
		unsigned long long n_fra = 0;
		int fra_size = 0;

		file.read((char*)&n_fra,    sizeof(n_fra));
		file.read((char*)&fra_size, sizeof(fra_size));

		assert(n_fra > 0 && fra_size > 0);

		this->noise.resize(n_fra);
		for (unsigned i = 0; i < (unsigned)n_fra; i++)
			this->noise[i].resize(fra_size);

		if (fra_size == this->N)
		{
			for (unsigned i = 0; i < (unsigned)n_fra; i++)
				file.read(reinterpret_cast<char*>(&this->noise[i][0]), fra_size*sizeof(R));
		}
		else
		{
			std::cerr << bold_red("(EE) The frame size is wrong (read: ") << bold_red(std::to_string(fra_size))
			          << bold_red(", expected: ") << bold_red(std::to_string(this->N))
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

template <typename R>
Channel_user<R>
::~Channel_user()
{
}

template <typename R>
void Channel_user<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert((int)X_N.size() == this->N * this->n_frames);

	for (auto f = 0; f < this->n_frames; f++)
	{
		for (auto i = 0; i < this->N; i++)
			Y_N[f * this->N +i] = this->noise[this->noise_counter][i];

		this->noise_counter = (this->noise_counter +1) % (int)this->noise.size();
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_user<R_32>;
template class aff3ct::module::Channel_user<R_64>;
#else
template class aff3ct::module::Channel_user<R>;
#endif
// ==================================================================================== explicit template instantiation
