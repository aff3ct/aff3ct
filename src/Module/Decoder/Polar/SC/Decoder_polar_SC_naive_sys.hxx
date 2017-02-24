#include "Decoder_polar_SC_naive_sys.hpp"

namespace aff3ct
{
template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>
::Decoder_polar_SC_naive_sys(const int& K, const int& N, const mipp::vector<B>& frozen_bits, const int n_frames,
                             const std::string name)
: Decoder_polar_SC_naive<B,R,F,G,H>(K, N, frozen_bits, n_frames, name)
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>
::~Decoder_polar_SC_naive_sys()
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
void Decoder_polar_SC_naive_sys<B,R,F,G,H>
::store(mipp::vector<B>& V_K) const
{
	auto *contents_root = this->polar_tree.get_root()->get_c();
	assert(V_K.size() >= (unsigned) this->K);

	auto k = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			V_K[k++] = contents_root->s[i];
}
}
