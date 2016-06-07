#include <algorithm>
#include <cmath>
#include <map>

#include <iostream>
#include <fstream>

#include "Decoder_polar_SCL_naive_sys.hpp"

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
Decoder_polar_SCL_naive_sys<B,R,F,G>
::Decoder_polar_SCL_naive_sys(const int& N, const int& L, const mipp::vector<B>& frozen_bits)
: Decoder_polar_SCL_naive<B,R,F,G>(N, L, frozen_bits)
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
void Decoder_polar_SCL_naive_sys<B,R,F,G>
::store(mipp::vector<B>& V_N) const
{
	assert(V_N.size() == (unsigned) this->N);

	V_N = this->polar_trees[*this->active_paths.begin()]->get_root()->get_c()->s;
}


