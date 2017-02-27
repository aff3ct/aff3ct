#ifndef DECODER_TURBO_NAIVE_CA_
#define DECODER_TURBO_NAIVE_CA_

#include "../../../CRC/CRC.hpp"

#include "../Decoder_turbo_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_naive_CA : public Decoder_turbo_naive<B,R>
{
protected:
	CRC<B> &crc;

public:
	Decoder_turbo_naive_CA(const int& K,
	                       const int& N_without_tb,
	                       const int& n_ite,
	                       const Interleaver<short> &pi,
	                       SISO<R> &siso_n,
	                       SISO<R> &siso_i,
	                       tools::Scaling_factor<R> &scaling_factor,
	                       CRC<B> &crc,
	                       const bool buffered_encoding = true);
	virtual ~Decoder_turbo_naive_CA();

protected:
	virtual void hard_decode();
};
}
}

#endif /* DECODER_TURBO_NAIVE_CA_ */
