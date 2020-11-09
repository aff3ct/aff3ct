/*!
 * \file
 * \brief Class module::Decoder_turbo_fast.
 */
#ifndef DECODER_TURBO_FAST_SYS_
#define DECODER_TURBO_FAST_SYS_

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_turbo_fast : public Decoder_turbo<B,R>
{
public:
	Decoder_turbo_fast(const int& K,
	                   const int& N,
	                   const int& n_ite,
	                   const Decoder_SISO<B,R> &siso_n,
	                   const Decoder_SISO<B,R> &siso_i,
	                   const Interleaver<R> &pi,
	                   const bool buffered_encoding = true);
	virtual ~Decoder_turbo_fast() = default;
	virtual Decoder_turbo_fast<B,R>* clone() const;

protected:
	void _load       (const R *Y_N,         const size_t frame_id);
	int  _decode_siho(const R *Y_N, B *V_K, const size_t frame_id);
	void _store      (              B *V_K                       ) const;
};
}
}

#endif /* DECODER_TURBO_FAST_SYS_ */
