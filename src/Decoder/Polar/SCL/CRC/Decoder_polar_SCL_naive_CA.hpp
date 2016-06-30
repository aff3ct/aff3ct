#ifndef DECODER_POLAR_SCL_NAIVE_CA_
#define DECODER_POLAR_SCL_NAIVE_CA_

#include "../../../../CRC/CRC.hpp"

#include "../Decoder_polar_SCL_naive.hpp"

template <typename B, typename R, proto_f<R> F, proto_g<B,R> G>
class Decoder_polar_SCL_naive_CA : public Decoder_polar_SCL_naive<B,R,F,G>
{
protected:
	CRC<B>& crc;

public:
	Decoder_polar_SCL_naive_CA(const int& K, const int& N, const int& L, const mipp::vector<B>& frozen_bits, CRC<B>& crc);

protected:
	virtual void select_best_path();
};

#include "Decoder_polar_SCL_naive_CA.hxx"

#endif /* DECODER_POLAR_SCL_NAIVE_CA_ */
