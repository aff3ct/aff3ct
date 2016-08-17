#include <algorithm>
#include <cmath>
#include <map>

#include <iostream>
#include <fstream>

#include "Decoder_polar_SCL_naive_sys.hpp"

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
Decoder_polar_SCL_naive_sys<B,R,F,G>
::Decoder_polar_SCL_naive_sys(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits,
                              const std::string name)
: Decoder_polar_SCL_naive<B,R,F,G>(K, N, L, frozen_bits, name)
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive_sys<B,R,F,G>
::store(mipp::vector<B>& V_K) const
{
	assert(V_K.size() == (unsigned) this->K);

	auto k = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			V_K[k++] = this->polar_trees[*this->active_paths.begin()]->get_root()->get_c()->s[i] ? 1 : 0;
}


