/*!
 * \file
 * \brief Class module::Decoder_turbo_std.
 */
#ifndef DECODER_TURBO_NAIVE_HPP
#define DECODER_TURBO_NAIVE_HPP

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/Turbo/Decoder_turbo.hpp"

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
	                  const Decoder_SISO<B,R> &siso_n,
	                  const Decoder_SISO<B,R> &siso_i,
	                  const Interleaver<R> &pi,
	                  const bool buffered_encoding = true);
	virtual ~Decoder_turbo_std() = default;
	virtual Decoder_turbo_std<B,R>* clone() const;

protected:
	virtual int _decode_siho(const R *Y_N, B *V_K, const size_t frame_id);
};
}
}

#endif /* DECODER_TURBO_NAIVE_HPP */
