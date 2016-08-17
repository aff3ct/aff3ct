#ifndef DECODER_TURBO_HPP_
#define DECODER_TURBO_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../../Interleaver/Interleaver.hpp"

#include "../Decoder.hpp"
#include "../SISO.hpp"

#include "../../Tools/Turbo/Scaling_factor/Scaling_factor.hpp"

template <typename B, typename R>
class Decoder_turbo : public Decoder<B,R>
{
protected:
	const int  K;     // n info bits
	const int  N;     // n bits input
	const int  n_ite; // number of iterations
	const bool buffered_encoding;

	const Interleaver<short> &pi;
	SISO<R> &siso_n;
	SISO<R> &siso_i;

	Scaling_factor<R>& scaling_factor;

	mipp::vector<R> l_sn;  // systematic LLRs                  in the natural     domain
	mipp::vector<R> l_si;  // systematic LLRs                  in the interleaved domain
	mipp::vector<R> l_sen; // systematic LLRs + extrinsic LLRs in the natural     domain
	mipp::vector<R> l_sei; // systematic LLRs + extrinsic LLRs in the interleaved domain
	mipp::vector<R> l_pn;  // parity     LLRs                  in the natural     domain
	mipp::vector<R> l_pi;  // parity     LLRs                  in the interleaved domain
	mipp::vector<R> l_e1n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e2n; // extrinsic  LLRs                  in the natural     domain
	mipp::vector<R> l_e1i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<R> l_e2i; // extrinsic  LLRs                  in the interleaved domain
	mipp::vector<B> s;     // bit decision

public:
	Decoder_turbo(const int& K,
	              const int& N,
	              const int& n_ite,
	              const Interleaver<short> &pi,
	              SISO<R> &siso_n,
	              SISO<R> &siso_i,
	              Scaling_factor<R> &scaling_factor,
	              const bool buffered_encoding = true,
	              const std::string name = "Decoder_turbo");
	virtual ~Decoder_turbo();

	virtual void load (const mipp::vector<R>& Y_N);
	virtual void store(      mipp::vector<B>& V_K) const;

	int get_n_frames() const { return siso_n.get_n_frames(); }

private:
	void buffered_load(const mipp::vector<R>& Y_N);
	void standard_load(const mipp::vector<R>& Y_N);
};

#include "Decoder_turbo.hxx"

#endif /* DECODER_TURBO_HPP_ */
