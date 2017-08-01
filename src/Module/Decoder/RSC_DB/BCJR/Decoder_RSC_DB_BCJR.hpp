#ifndef DECODER_RSC_DB_BCJR_HPP_
#define DECODER_RSC_DB_BCJR_HPP_

#include <vector>
#include <string>
#include <mipp.h>

#include "../../Decoder_SISO.hpp"

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

	mipp::vector<R> sys, par;          // input LLR from the channel
	mipp::vector<R> ext;               // extrinsic LLRs
	mipp::vector<B> s;                 // hard decision
	mipp::vector<R> alpha_mp, beta_mp; // message passing
	mipp::vector<mipp::vector<R>> alpha, beta, gamma;

public:
	Decoder_RSC_DB_BCJR(const int K,
	                    const std::vector<std::vector<int>> &trellis,
	                    const bool buffered_encoding = true,
	                    const int n_frames = 1,
	                    const std::string name = "Decoder_RSC_DB_BCJR");
	virtual ~Decoder_RSC_DB_BCJR();

	void notify_new_frame();

protected:
	virtual void _load            (const R *Y_N                                          );
	virtual void _store           (              B *V_K                                  ) const;
	virtual void _hard_decode     (const R *Y_N, B *V_K,               const int frame_id);
	virtual void _soft_decode     (const R *sys, const R *par, R *ext, const int frame_id);
	        void __init_alpha_beta(                                                      );
	        void __save_alpha_beta(                                                      );
	virtual void __fwd_recursion  (const R *sys, const R *par                            ) = 0;
	virtual void __bwd_recursion  (const R *sys, const R *par, R* ext                    ) = 0;
};
}
}

#endif /* DECODER_RSC_DB_BCJR_HPP_ */
