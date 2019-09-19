#ifndef ENCODER_TURBO_HPP_
#define ENCODER_TURBO_HPP_

#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo : public Encoder<B>
{
protected:
	const Interleaver<B> &pi; // the interleaver

	Encoder<B> &enco_n;       // polar systematic encoder
	Encoder<B> &enco_i;       // sub encoder

	std::vector<B> U_K_i;     // internal buffer for the systematic bits in the interleaved domain
	std::vector<B> X_N_tmp;

public:
	Encoder_turbo(const int& K, const int& N, const Interleaver<B> &pi, Encoder<B> &enco_n, Encoder<B> &enco_i);
	virtual ~Encoder_turbo() = default;

	int tail_length() const;

	virtual bool is_codeword(const B *X_N);

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_TURBO_HPP_
