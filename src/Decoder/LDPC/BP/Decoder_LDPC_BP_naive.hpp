#ifndef DECODER_LDPC_BP_NAIVE_HPP_
#define DECODER_LDPC_BP_NAIVE_HPP_

#include "../../Decoder.hpp"
#include "../../SISO.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_naive : public Decoder<B,R>, public SISO<R>
{
private:
	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;
	const int n_ite;
	
public:
	Decoder_LDPC_BP_naive(const int &N, const int &K, const int& n_ite);
	virtual ~Decoder_LDPC_BP_naive();

	// unsupported prototype
	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext );

	// soft decode
	void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

	// hard decoder (load -> decode -> store)
	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;
};

#endif /* DECODER_LDPC_BP_NAIVE_HPP_ */