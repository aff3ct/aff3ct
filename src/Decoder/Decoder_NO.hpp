#ifndef DECODER_NO_HPP_
#define DECODER_NO_HPP_

#include "Decoder.hpp"

template <typename B, typename R>
class Decoder_NO : public Decoder<B,R>
{
private:
	mipp::vector<R> Y_N;
	
public:
	Decoder_NO();
	virtual ~Decoder_NO();

	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;
};

#endif /* DECODER_NO_HPP_ */