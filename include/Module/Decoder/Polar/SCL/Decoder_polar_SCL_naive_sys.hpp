#ifndef DECODER_POLAR_SCL_NAIVE_SYS
#define DECODER_POLAR_SCL_NAIVE_SYS

#include <vector>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive_sys : public Decoder_polar_SCL_naive<B,R,F,G>
{
public:
	Decoder_polar_SCL_naive_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                            const int n_frames = 1);

	virtual ~Decoder_polar_SCL_naive_sys() = default;

protected:
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive_sys.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_SYS */
