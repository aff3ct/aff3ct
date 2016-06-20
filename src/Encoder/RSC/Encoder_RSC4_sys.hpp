#ifndef ENCODER_RSC4_SYS_HPP_
#define ENCODER_RSC4_SYS_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Encoder_RSC.hpp"

template <typename B>
class Encoder_RSC4_sys : public Encoder_RSC<B>
{
protected:
	const int K; // info bits
	const int N; // code length

	const bool buffered_encoding;

public:
	Encoder_RSC4_sys(const int& K, const int& N, const int& n_frames = 1, const bool buffered_encoding = true);
	virtual ~Encoder_RSC4_sys() {}

	virtual void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);

	mipp::vector<mipp::vector<int>> get_trellis();

protected:
	inline int inner_enc(const int bit_sys, int &state);
	void frame_encode(const B* U_K, B* X_N, const int stride = 1, const bool only_parity = false);
};

#endif // ENCODER_RSC4_SYS_HPP_
