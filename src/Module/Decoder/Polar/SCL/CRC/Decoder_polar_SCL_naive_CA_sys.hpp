#ifndef DECODER_POLAR_SCL_NAIVE_CA_SYS_
#define DECODER_POLAR_SCL_NAIVE_CA_SYS_

#include "Tools/Code/Polar/decoder_polar_functions.h"

#include "Decoder_polar_SCL_naive_CA.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive_CA_sys : public Decoder_polar_SCL_naive_CA<B,R,F,G>
{

public:
	Decoder_polar_SCL_naive_CA_sys(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                               CRC<B>& crc, const int n_frames = 1,
	                               const std::string name = "Decoder_polar_SCL_naive_CA_sys");

	virtual ~Decoder_polar_SCL_naive_CA_sys() {}

protected:
	virtual void select_best_path();
	virtual void _store(B *V, bool coded = false) const;
};
}
}

#include "Decoder_polar_SCL_naive_CA_sys.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_CA_SYS_ */
