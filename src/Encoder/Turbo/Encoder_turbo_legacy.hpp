#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include <string>

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_turbo_legacy : public Encoder<B>
{
protected:
	const int K;                  // info bits
	const int N;                  // code length
	const Interleaver<short> &pi; // the interleaver

	Encoder_sys <B> &sub_enc; // sub encoder
	mipp::vector<B>  U_K_i;   // internal buffer for the systematic bits in the interleaved domain
	mipp::vector<B>  X_N_n;   // internal buffer for the encoded    bits in the natural     domain
	mipp::vector<B>  X_N_i;   // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<short> &pi, Encoder_sys<B> &sub_enc, 
	                     const int n_frames = 1, const std::string name = "Encoder_turbo_legacy");
	virtual ~Encoder_turbo_legacy() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);

	void set_n_frames(const int n_frames) 
	{ 
		assert(n_frames > 0);
		Encoder<B>::set_n_frames(n_frames);
		sub_enc.set_n_frames(n_frames);

		U_K_i.resize(K * n_frames);
		X_N_n.resize((2 * (K + sub_enc.tail_length()/2)) * n_frames);
		X_N_i.resize((2 * (K + sub_enc.tail_length()/2)) * n_frames);
	}
};

#endif // ENCODER_TURBO_LEGACY_HPP_
