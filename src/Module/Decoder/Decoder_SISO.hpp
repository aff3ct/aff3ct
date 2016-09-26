#ifndef DECODER_SISO_HPP_
#define DECODER_SISO_HPP_

#include "Decoder.hpp"
#include "SISO.hpp"

template <typename B, typename R>
class Decoder_SISO : public Decoder<B,R>, public SISO<R>
{
public:
	Decoder_SISO(const int K, const int N, const int n_frames = 1, std::string name = "Decoder_SISO") 
	: Decoder<B,R>(K, N, n_frames, name          ), 
	  SISO   <  R>(K, N, n_frames, name + "_siso") {}
	virtual ~Decoder_SISO() {}

	// virtual methods from the Decoder
	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

	virtual void store_fast(mipp::vector<B>& V) const { Decoder<B,R>::store_fast(V); }
	virtual void unpack    (mipp::vector<B>& V) const { Decoder<B,R>::unpack    (V); }

	// virtual methods from the SISO
	virtual void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;
	virtual void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2) = 0;
	
	virtual int tail_length() const { return SISO<R>::tail_length(); }
};

#endif /* DECODER_SISO_HPP_ */