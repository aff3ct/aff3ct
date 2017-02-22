#include <fstream>
#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "Channel_additive_user.hpp"

template <typename R>
Channel_additive_user<R>
::Channel_additive_user(const int N, const std::string filename, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name), noise(), noise_counter(0)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (file.is_open())
	{
		int n_fra = 0, fra_size = 0;

		file.read((char *)&n_fra,    sizeof(int));
		file.read((char *)&fra_size, sizeof(int));

		assert(n_fra > 0 && fra_size > 0);

		this->noise.resize(n_fra);
		for (auto i = 0; i < n_fra; i++)
			this->noise[i].resize(fra_size);

		if (fra_size == this->N)
		{
			for (auto i = 0; i < n_fra; i++)
				for (auto j = 0; j < fra_size; j++)
				{
					R val;
					file.read((char *)&val, sizeof(float));
					this->noise[i][j] = val;
				}
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

	//std::ifstream file(filename.c_str(), std::ios::in);
//	if (file.is_open())
//	{
//		int n_fra = 0, fra_size = 0;
//
//		file >> n_fra;
//		file >> fra_size;
//
//		assert(n_fra > 0 && fra_size > 0);
//
//		this->noise.resize(n_fra);
//		for (auto i = 0; i < n_fra; i++)
//			this->noise[i].resize(fra_size);
//
//		if (fra_size == this->N)
//		{
//			for (auto i = 0; i < n_fra; i++)
//				for (auto j = 0; j < fra_size; j++)
//				{
//					R val;
//					file >> val;
//					this->noise[i][j] = val;
//				}
//		}
//		else
//		{
//			std::cerr << bold_red("(EE) The frame size is wrong (read: ") << bold_red(std::to_string(fra_size))
//			          << bold_red(", expected: ") << bold_red(std::to_string(this->N))
//			          << bold_red("), exiting.") << std::endl;
//			file.close();
//			std::exit(-1);
//		}
//
//		file.close();
//	}
//	else
//	{
//		std::cerr << bold_red("(EE) Can't open \"") << bold_red(filename) << bold_red("\" file, exiting.")
//		          << std::endl;
//		std::exit(-1);
//	}
}

template <typename R>
Channel_additive_user<R>
::~Channel_additive_user()
{
}

template <typename R>
void Channel_additive_user<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert((int)X_N.size() == this->N * this->n_frames);

	for (auto f = 0; f < this->n_frames; f++)
	{
		for (auto i = 0; i < this->N; i++)
			Y_N[f * this->N +i] = X_N[f * this->N +i] + this->noise[this->noise_counter][i];

		this->noise_counter = (this->noise_counter +1) % (int)this->noise.size();
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Channel_additive_user<R_32>;
template class Channel_additive_user<R_64>;
#else
template class Channel_additive_user<R>;
#endif
// ==================================================================================== explicit template instantiation
