#include <algorithm>
#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "Channel_Rayleigh_LLR.hpp"

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const R& sigma, const bool complex, const int seed, const int n_frames,
                       const std::string name)
: Channel<R>(N, n_frames, name.c_str()),
  sigma(sigma),
  complex(complex),
  rd(),
  rd_engine(this->rd()),
  normal_dist_n(0, sigma),
  normal_dist_h(0, (R)1 / (R)std::sqrt((R)2))
{
	assert(sigma != 0);
	rd_engine.seed(seed);
}

template <typename R>
Channel_Rayleigh_LLR<R>
::~Channel_Rayleigh_LLR()
{
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	std::cerr << bold_red("(EE) Adding noise without computing the gains is not possible with the Rayleigh channel.")
	          << std::endl;
	std::exit(-1);
}

template <typename R>
void Channel_Rayleigh_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N, mipp::vector<R>& H_N)
{
	assert(X_N.size() == Y_N.size());
	assert(X_N.size() == H_N.size());

	if (this->complex)
	{
		assert((int)X_N.size() % 2 == 0);
		for (auto i = 0; i < (int)X_N.size() / 2 ; i++)
		{
			H_N[2*i   ] = this->normal_dist_h(this->rd_engine);
			H_N[2*i +1] = this->normal_dist_h(this->rd_engine);

			const auto n_re = this->normal_dist_n(this->rd_engine);
			const auto n_im = this->normal_dist_n(this->rd_engine);

			Y_N[2*i   ] = H_N[2*i] * X_N[2*i   ] - H_N[2*i +1] * X_N[2*i +1] + n_re;
			Y_N[2*i +1] = H_N[2*i] * X_N[2*i +1] + H_N[2*i +1] * X_N[2*i   ] + n_im;
		}
	}
	else
	{
		for (auto i = 0; i < (int)X_N.size() ; i++)
		{
			const auto h_re = this->normal_dist_h(this->rd_engine);
			const auto h_im = this->normal_dist_h(this->rd_engine);

			H_N[i] = std::sqrt(h_re * h_re + h_im * h_im);
			Y_N[i] = H_N[i] * X_N[i] +  this->normal_dist_n(this->rd_engine);
		}
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Channel_Rayleigh_LLR<R_32>;
template class Channel_Rayleigh_LLR<R_64>;
#else
template class Channel_Rayleigh_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation
