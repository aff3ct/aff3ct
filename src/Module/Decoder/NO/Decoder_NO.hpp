#ifndef DECODER_NO_HPP_
#define DECODER_NO_HPP_

#include "../Decoder.hpp"
#include "../SISO.hpp"

template <typename B, typename R>
class Decoder_NO : public Decoder<B,R>, public SISO<R>
{
private:
	mipp::vector<R> Y_N;
	
public:
	Decoder_NO(const int K, const int N, const std::string name = "Decoder_NO");
	virtual ~Decoder_NO();

	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;

	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext );
	void decode(const mipp::vector<R> &Y_N1,                            mipp::vector<R> &Y_N2);
};

#endif /* DECODER_NO_HPP_ */