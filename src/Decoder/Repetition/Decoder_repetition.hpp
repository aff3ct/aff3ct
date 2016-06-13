#ifndef DECODER_REPETITION
#define DECODER_REPETITION

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Decoder.hpp"

template <typename B, typename R>
class Decoder_repetition : public Decoder<B,R>
{
protected:
	const int K; // n info bits
	const int N; // n bits input
	const int rep_count; // number of repetitions
	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;

public:
	Decoder_repetition(const int& K, const int& N);
	virtual ~Decoder_repetition();

	        void load  (const mipp::vector<R>& Y_N);
	virtual void decode(                          ) = 0;
	        void store (      mipp::vector<B>& V_K) const;
};

#include "Decoder_repetition.hxx"

#endif /* DECODER_REPETITION */
