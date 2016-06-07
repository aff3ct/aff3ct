#include "Decoder_polar_SC_naive_sys.hpp"

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>
::Decoder_polar_SC_naive_sys(const int& N, const mipp::vector<B>& frozen_bits)
: Decoder_polar_SC_naive<B,R,F,G,H>(N, frozen_bits)
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>
::~Decoder_polar_SC_naive_sys()
{
}

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G, proto_h<B,R> H>
void Decoder_polar_SC_naive_sys<B,R,F,G,H>
::store(mipp::vector<B>& V_N) const
{
	auto *contents_root = this->polar_tree.get_root()->get_c();
	assert(V_N.size() == contents_root->s.size());

	for (unsigned i = 0; i < V_N.size(); i++)
		V_N[i] = contents_root->s[i];
}
