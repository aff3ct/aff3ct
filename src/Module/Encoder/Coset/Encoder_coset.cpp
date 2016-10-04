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
	assert(this->n_frames == 1);
	assert(this->N == static_cast<int>(X_N.size()));

	std::copy(U_K.begin(), U_K.end(), X_N.begin());

	for (auto i = (int)U_K.size(); i < this->N; i++)
		X_N[i] = (B)this->uniform_dist(this->rd_engine);
}

template <typename B>
void Encoder_coset<B>
::encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par)
{
	assert(this->n_frames == 1);
	assert(this->N - this->K == static_cast<int>(par.size()));

	for (auto i = 0; i < this->N - this->K; i++)
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
