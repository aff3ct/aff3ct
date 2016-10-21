#include "Channel_Rayleigh_LLR.hpp"

#include <algorithm>
#include <cassert>

template <typename R>
Channel_Rayleigh_LLR<R>
::Channel_Rayleigh_LLR(const int N, const R& sigma, const bool is_complex, mipp::vector<R> &H, 
                       const int seed, const int n_frames, const std::string name)
: Channel<R>(N, n_frames, name.c_str()),
  sigma(sigma),
  is_complex(is_complex),
  H(H),
  rd(),
  rd_engine(this->rd()),
  normal_dist_n(0, sigma),
  normal_dist_h(0, 1/sqrt(2))
{
	assert(sigma != 0);
	rd_engine.seed(seed);
	H.resize(N);
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
	R n_re, n_im;

	if(this->is_complex)
	{

		for (unsigned i = 0; i < X_N.size() / 2 ; i++)
		{
			H[2*i   ] = this->normal_dist_h(this->rd_engine);
			H[2*i +1] = this->normal_dist_h(this->rd_engine);

			n_re = this->normal_dist_n(this->rd_engine);
			n_im = this->normal_dist_n(this->rd_engine);

			Y_N[2*i   ] = H[2*i] * X_N[2*i   ] - H[2*i +1] * X_N[2*i +1] + n_re;
			Y_N[2*i +1] = H[2*i] * X_N[2*i +1] + H[2*i +1] * X_N[2*i   ] + n_im;
		}
	}
	else
	{
		for (unsigned i = 0; i < X_N.size() ; i++)
		{
			R h_re = this->normal_dist_h(this->rd_engine);
			R h_im = this->normal_dist_h(this->rd_engine);
			H[i]   = sqrt(h_re * h_re + h_im * h_im);
			Y_N[i] = H[i] * X_N[i] +  this->normal_dist_n(this->rd_engine);
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
