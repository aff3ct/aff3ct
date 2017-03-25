#include <fstream>
#include <stdexcept>

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

		if (n_fra <= 0 || fra_size <= 0)
			throw std::runtime_error("aff3ct::module::Channel_user: \"n_fra\" and \"fra_size\" have to be "
			                         "bigger than 0.");

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
			file.close();

			throw std::runtime_error("aff3ct::module::Channel_user: the frame size is wrong (read: " +
			                         std::to_string(fra_size) + ", expected: " + std::to_string(this->N) + ").");
		}

		file.close();
	}
	else
	{
		throw std::invalid_argument("aff3ct::module::Channel_user: can't open \"" + filename + "\" file.");
	}
}

template <typename R>
Channel_user<R>
::~Channel_user()
{
}

template <typename R>
void Channel_user<R>
::_add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
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
