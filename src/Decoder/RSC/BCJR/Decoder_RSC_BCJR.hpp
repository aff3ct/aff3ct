#ifndef DECODER_RSC_BCJR_HPP_
#define DECODER_RSC_BCJR_HPP_

#include <vector>
#include "../../../Tools/MIPP/mipp.h"

#include "../../Decoder.hpp"
#include "../../SISO.hpp"

template <typename B, typename R>
class Decoder_RSC_BCJR : public Decoder<B,R>, public SISO<R>
{
protected:
	const int  n_states;
	const int  n_ff;
	const bool buffered_encoding;
	const int  n_frames;

	const std::vector<std::vector<int>> &trellis;

	mipp::vector<R> sys, par; // input LLR from the channel
	mipp::vector<R> ext;      // extrinsic LLRs
	mipp::vector<B> s;        // hard decision

public:
	Decoder_RSC_BCJR(const int K, 
	                 const std::vector<std::vector<int>> &trellis, 
	                 const bool buffered_encoding = true, 
	                 const int n_frames = 1,
	                 const std::string name = "Decoder_RSC_BCJR");
	virtual ~Decoder_RSC_BCJR();

	virtual void load  (const mipp::vector<R>& Y_N);
	        void decode(                          );
	virtual void store (      mipp::vector<B>& V_K) const;

	int get_n_frames() const { return n_frames; };
	
	virtual int tail_length () const { return 2 * n_ff; }

	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	virtual void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);
};

#include "Decoder_RSC_BCJR.hxx"

#endif /* DECODER_RSC_BCJR_HPP_ */
