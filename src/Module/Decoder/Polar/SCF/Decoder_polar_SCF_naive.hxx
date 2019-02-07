#include "Decoder_polar_SCF_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F, tools::proto_g<B,R> G, tools::proto_h<B,R> H>
Decoder_polar_SCF_naive<B,R,F,G,H>
::Decoder_polar_SCF_naive(const int& K, const int& N, const std::vector<bool>& frozen_bits, const int n_frames)
: Decoder(K, N, n_frames, 1),
  Decoder_polar_SC_naive<B,R,F,G,H>(K, N, frozen_bits, n_frames)
{
	const std::string name = "Decoder_polar_SCF_naive";
	this->set_name(name);
}
}
}
