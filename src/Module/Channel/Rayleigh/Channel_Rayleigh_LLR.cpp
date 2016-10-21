#include "Channel_Rayleigh_LLR.hpp"

#include <algorithm>
#include <cassert>

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const R& sigma, const int seed, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name.c_str()),
  sigma(sigma),
  rd(),
  rd_engine(this->rd()),
  normal_dist_n(0, sigma),
  normal_dist_h(0, 1/sqrt(2))
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
	assert(X_N.size() == Y_N.size());
	R n_re, n_im, h_re, h_im, n_re_eq, n_re_im;

	for (unsigned i = 0; i < X_N.size() / 2 ; i++)
	{
		n_re = this->normal_dist_n(this->rd_engine);
		n_im = this->normal_dist_n(this->rd_engine);
		h_re = this->normal_dist_h(this->rd_engine);
		h_im = this->normal_dist_h(this->rd_engine);

		n_re_eq = (n_re * h_re + n_im * h_im) / (h_re * h_re + h_im * h_im);
		n_re_im = (n_im * h_re - n_re * h_im) / (h_re * h_re + h_im * h_im);

		Y_N[2*i    ] = X_N[2*i    ] + n_re_eq;
		Y_N[2*i + 1] = X_N[2*i + 1] + n_re_im;
	}

	if ((X_N.size() % 2) == 1)
	{
		n_re = this->normal_dist_n(this->rd_engine);
		n_im = this->normal_dist_n(this->rd_engine);
		h_re = this->normal_dist_h(this->rd_engine);
		h_im = this->normal_dist_h(this->rd_engine);
		n_re_eq = (n_re * h_re + n_im * h_im) / (h_re * h_re + h_im * h_im);

		Y_N[X_N.size()- 1] = X_N[X_N.size()- 1] + n_re_eq;
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
