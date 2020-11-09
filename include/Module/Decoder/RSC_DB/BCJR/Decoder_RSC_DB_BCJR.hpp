/*!
 * \file
 * \brief Class module::Decoder_RSC_DB_BCJR.
 */
#ifndef DECODER_RSC_DB_BCJR_HPP_
#define DECODER_RSC_DB_BCJR_HPP_

#include <vector>

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_DB_BCJR : public Decoder_SISO<B,R>
{
protected:
	const int  n_states;
	const int  n_ff;
	const bool buffered_encoding;

	const std::vector<std::vector<int>> &trellis;

	std::vector<R> sys, par;          // input LLR from the channel
	std::vector<R> ext;               // extrinsic LLRs
	std::vector<B> s;                 // hard decision
	std::vector<R> alpha_mp, beta_mp; // message passing
	std::vector<std::vector<R>> alpha, beta, gamma;

public:
	Decoder_RSC_DB_BCJR(const int K,
	                    const std::vector<std::vector<int>> &trellis,
	                    const bool buffered_encoding = true);
	virtual ~Decoder_RSC_DB_BCJR() = default;
	virtual Decoder_RSC_DB_BCJR<B,R>* clone() const;

	void notify_new_frame();

protected:
	virtual void _load            (const R *Y_N                                             );
	virtual void _store           (              B *V_K                                     ) const;
	virtual int  _decode_siho     (const R *Y_N, B *V_K,               const size_t frame_id);
	virtual int  _decode_siso_alt (const R *sys, const R *par, R *ext, const size_t frame_id);
	        void __init_alpha_beta(                                                         );
	        void __save_alpha_beta(                                                         );
	virtual void __fwd_recursion  (const R *sys, const R *par                               ) = 0;
	virtual void __bwd_recursion  (const R *sys, const R *par, R* ext                       ) = 0;
};
}
}

#endif /* DECODER_RSC_DB_BCJR_HPP_ */
