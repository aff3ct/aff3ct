/*!
 * \file
 * \brief Class module::Decoder_RSC_BCJR.
 */
#ifndef DECODER_RSC_BCJR_HPP_
#define DECODER_RSC_BCJR_HPP_

#include <vector>
#include <mipp.h>

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_BCJR : public Decoder_SISO<B,R>
{
protected:
	const int  n_states;
	const int  n_ff;
	const bool buffered_encoding;

	const std::vector<std::vector<int>> trellis;

	mipp::vector<R> sys, par; // input LLR from the channel
	mipp::vector<R> ext;      // extrinsic LLRs
	mipp::vector<B> s;        // hard decision

	Decoder_RSC_BCJR(const int K,
	                 const std::vector<std::vector<int>> &trellis,
	                 const bool buffered_encoding = true);
	virtual ~Decoder_RSC_BCJR() = default;
	virtual Decoder_RSC_BCJR<B,R>* clone() const;

public:
	virtual int tail_length() const;

protected:
	virtual void set_n_frames_per_wave(const size_t n_frames_per_wave);

	virtual void _load       (const R *Y_N                               );
	        int  _decode_siho(const R *Y_N, B *V_K, const size_t frame_id);
	virtual void _store      (              B *V_K                       ) const;
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/RSC/BCJR/Decoder_RSC_BCJR.hxx"
#endif

#endif /* DECODER_RSC_BCJR_HPP_ */
