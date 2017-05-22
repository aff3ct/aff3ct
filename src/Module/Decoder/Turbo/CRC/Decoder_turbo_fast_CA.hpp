#ifndef DECODER_TURBO_FAST_CA_HPP_
#define DECODER_TURBO_FAST_CA_HPP_

#include "../../../CRC/CRC.hpp"

#include "../Decoder_turbo_fast.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_fast_CA : public Decoder_turbo_fast<B,R>
{
protected:
	CRC<B> &crc;

public:
	Decoder_turbo_fast_CA(const int& K,
	                      const int& N,
	                      const int& n_ite,
	                      const Interleaver<int> &pi,
	                      SISO<R> &siso_n,
	                      SISO<R> &siso_i,
	                      tools::Scaling_factor<R> &scaling_factor,
	                      CRC<B> &crc,
	                      const bool buffered_encoding = true);
	virtual ~Decoder_turbo_fast_CA();

protected:
	virtual void _hard_decode(const R *Y_N, B *V_K, const int frame_id);
};
}
}

#endif /* DECODER_TURBO_FAST_CA_HPP_ */
