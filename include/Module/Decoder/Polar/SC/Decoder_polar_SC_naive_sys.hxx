#include <string>
#include <algorithm>

#include "Module/Decoder/Polar/SC/Decoder_polar_SC_naive_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>
::Decoder_polar_SC_naive_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits)
: Decoder_polar_SC_naive<B,R,F,G,H>(K, N, frozen_bits)
{
	const std::string name = "Decoder_polar_SC_naive_sys";
	this->set_name(name);
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
Decoder_polar_SC_naive_sys<B,R,F,G,H>* Decoder_polar_SC_naive_sys<B,R,F,G,H>
::clone() const
{
	auto m = new Decoder_polar_SC_naive_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
void Decoder_polar_SC_naive_sys<B,R,F,G,H>
::_store(B *V, bool coded) const
{
	auto *contents_root = this->polar_tree.get_root()->get_c();

	if (!coded)
	{
		auto k = 0;
		for (auto i = 0; i < this->N; i++)
			if (!this->frozen_bits[i])
				V[k++] = contents_root->s[i];
	}
	else
		std::copy(contents_root->s.begin(), contents_root->s.begin() + this->N, V);
}
}
}
