#ifndef DECODER_POLAR_SCL_NAIVE_CA_
#define DECODER_POLAR_SCL_NAIVE_CA_

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Module/CRC/CRC.hpp"
#include "Module/Decoder/Polar/SCL/Decoder_polar_SCL_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, tools::proto_f<R> F = tools::f_LLR, tools::proto_g<B,R> G = tools::g_LLR>
class Decoder_polar_SCL_naive_CA : public Decoder_polar_SCL_naive<B,R,F,G>
{
protected:
	CRC<B>& crc;

public:
	Decoder_polar_SCL_naive_CA(const int& K, const int& N, const int& L, const std::vector<bool>& frozen_bits,
	                           CRC<B>& crc, const int n_frames = 1);

	virtual ~Decoder_polar_SCL_naive_CA() = default;

protected:
	virtual void select_best_path();
};
}
}

#include "Module/Decoder/Polar/SCL/CRC/Decoder_polar_SCL_naive_CA.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_CA_ */
