#ifndef ENCODER_TURBO_LEGACY_HPP_
#define ENCODER_TURBO_LEGACY_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include <string>

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"
#include "../RSC/Encoder_RSC.hpp"
#include "../RSC/Encoder_RSC3_sys.hpp"

template <typename B>
class Encoder_turbo_legacy : public Encoder<B>
{
protected:
	const int K;                  // info bits
	const int N;                  // code length
	const Interleaver<short> &pi; // the interleaver
	const int n_frames;           // number of frames to encode

	Encoder_RSC3_sys<B> rsc; // sub encoder
	mipp::vector<B> U_K_i;   // internal buffer for the systematic bits in the interleaved domain
	mipp::vector<B> X_N_n;   // internal buffer for the encoded    bits in the natural     domain
	mipp::vector<B> X_N_i;   // internal buffer for the encoded    bits in the interleaved domain

public:
	Encoder_turbo_legacy(const int& K, const int& N, const Interleaver<short> &pi, const int n_frames = 1);
	virtual ~Encoder_turbo_legacy() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#endif // ENCODER_TURBO_LEGACY_HPP_
