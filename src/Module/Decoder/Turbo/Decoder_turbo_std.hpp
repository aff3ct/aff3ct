#ifndef DECODER_TURBO_NAIVE_HPP
#define DECODER_TURBO_NAIVE_HPP

#include "Decoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_std : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_std(const int& K,
	                  const int& N,
	                  const int& n_ite,
	                  const Interleaver<R> &pi,
	                  Decoder_SISO<R> &siso_n,
	                  Decoder_SISO<R> &siso_i,
	                  const bool buffered_encoding = true);
	virtual ~Decoder_turbo_std() = default;

protected:
	virtual void _decode_siho(const R *Y_N, B *V_K, const int frame_id);
};
}
}

#endif /* DECODER_TURBO_NAIVE_HPP */
