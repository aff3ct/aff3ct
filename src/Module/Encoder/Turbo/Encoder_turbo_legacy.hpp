#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <vector>
#include "Tools/MIPP/mipp.h"

#include <string>

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_turbo_legacy : public Encoder<B>
{
protected:
	const Interleaver<short> &pi; // the interleaver

	Encoder_sys <B> &sub_enc; // sub encoder
	mipp::vector<B>  U_K_i;   // internal buffer for the systematic bits in the interleaved domain
	mipp::vector<B>  X_N_n;   // internal buffer for the encoded    bits in the natural     domain
	mipp::vector<B>  X_N_i;   // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N_without_tb, const Interleaver<short> &pi, Encoder_sys<B> &sub_enc, 
	                     const int n_frames = 1, const std::string name = "Encoder_turbo_legacy");
	virtual ~Encoder_turbo_legacy() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);

	void set_n_frames(const int n_frames);
};

#endif // ENCODER_TURBO_LEGACY_HPP_
