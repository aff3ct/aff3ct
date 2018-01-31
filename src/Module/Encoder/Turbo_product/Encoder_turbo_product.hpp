#ifndef ENCODER_TURBO_PRODUCT_HPP_
#define ENCODER_TURBO_PRODUCT_HPP_

#include <string>
#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_product : public Encoder<B>
{
protected:
	const Interleaver<B> &pi; // the interleaver

	Encoder<B> &enc_r;       // row encoder
	Encoder<B> &enc_c;       // col encoder

	const bool parity_extended;

	std::vector<B> X_N_i;     // internal buffer in the interleaved domain

public:
	Encoder_turbo_product(const Interleaver<B> &pi, Encoder<B> &enc_r, Encoder<B> &enc_c);
	virtual ~Encoder_turbo_product() {}

	virtual void _encode(const B *U_K, B *X_N, const int frame_id); using Encoder<B>::encode;
};
}
}

#endif // ENCODER_TURBO_PRODUCT_HPP_
