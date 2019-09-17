#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <vector>

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Turbo/Encoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_legacy : public Encoder_turbo<B>
{
protected:
	const Interleaver<B> &pi;      // the interleaver
	Encoder    <B>       &sub_enc; // sub encoder
	std::vector<B>        X_N_n;   // internal buffer for the encoded    bits in the natural     domain
	std::vector<B>        X_N_i;   // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<B> &pi, Encoder<B> &sub_enc);
	virtual ~Encoder_turbo_legacy() = default;

	bool is_codeword(const B *X_N);

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
};
}
}

#endif // ENCODER_TURBO_LEGACY_HPP_
