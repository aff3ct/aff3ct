#ifndef DECODER_RSC_DB_BCJR_HPP_
#define DECODER_RSC_DB_BCJR_HPP_

#include <vector>
#include <string>

#include "../../Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RSC_DB_BCJR : public Decoder_SISO_SIHO<B,R>
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
	                    const bool buffered_encoding = true,
	                    const int n_frames = 1);
	virtual ~Decoder_RSC_DB_BCJR() = default;

	void notify_new_frame();

protected:
	virtual void _load            (const R *Y_N                                          );
	virtual void _store           (              B *V_K                                  ) const;
	virtual void _decode_siho     (const R *Y_N, B *V_K,               const int frame_id);
	virtual void _decode_siso     (const R *sys, const R *par, R *ext, const int frame_id);
	        void __init_alpha_beta(                                                      );
	        void __save_alpha_beta(                                                      );
	virtual void __fwd_recursion  (const R *sys, const R *par                            ) = 0;
	virtual void __bwd_recursion  (const R *sys, const R *par, R* ext                    ) = 0;
};
}
}

#endif /* DECODER_RSC_DB_BCJR_HPP_ */
