#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "Encoder_coset.hpp"

template <typename B>
Encoder_coset<B>
::Encoder_coset(const int K, const int N, const int seed, const int n_frames, const std::string name)
: Encoder_sys<B>(K, N, n_frames, name), rd_engine(seed + 1024), uniform_dist(0, 1)
{
}

template <typename B>
Encoder_coset<B>
::~Encoder_coset()
{
}

template <typename B>
void Encoder_coset<B>
::encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N)
{
	assert(U_K.size() <= X_N.size());
	assert(this->K * this->n_frames == static_cast<int>(U_K.size()));
	assert(this->N * this->n_frames == static_cast<int>(X_N.size()));

	for (auto f = 0; f < this->n_frames; f++)
	{
		std::copy(U_K.begin() + (f+ 0) * this->K, U_K.begin() + (f +1) * this->K, X_N.begin() + f * this->N);

		for (auto i = this->K; i < this->N; i++)
			X_N[f * this->N + i] = (B)this->uniform_dist(this->rd_engine);
	}
}

template <typename B>
void Encoder_coset<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert((this->N - this->K) * this->n_frames == static_cast<int>(par.size()));

	for (auto i = 0; i < (int)par.size(); i++)
		par[i] = (B)this->uniform_dist(this->rd_engine);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_coset<B_8>;
template class Encoder_coset<B_16>;
template class Encoder_coset<B_32>;
template class Encoder_coset<B_64>;
#else
template class Encoder_coset<B>;
#endif
// ==================================================================================== explicit template instantiation
