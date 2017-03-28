#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include <string>

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../Encoder_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_turbo_legacy : public Encoder<B>
{
protected:
	Interleaver<int> &pi; // the interleaver

	Encoder_sys <B> &sub_enc; // sub encoder
	mipp::vector<B>  U_K_i;   // internal buffer for the systematic bits in the interleaved domain
	mipp::vector<B>  X_N_n;   // internal buffer for the encoded    bits in the natural     domain
	mipp::vector<B>  X_N_i;   // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N_without_tb, Interleaver<int> &pi, Encoder_sys<B> &sub_enc,
	                     const int n_frames = 1, const std::string name = "Encoder_turbo_legacy");
	virtual ~Encoder_turbo_legacy() {}

	int tail_length() const { return sub_enc.tail_length() + sub_enc.tail_length(); }

	void set_n_frames(const int n_frames);

protected:
	virtual void _encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};
}
}

#endif // ENCODER_TURBO_LEGACY_HPP_
