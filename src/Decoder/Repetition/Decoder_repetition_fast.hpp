#ifndef DECODER_REPETITION_FAST
#define DECODER_REPETITION_FAST

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Decoder_repetition.hpp"

template <typename B, typename R>
class Decoder_repetition_fast : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_fast(const int& K, const int& N);
	virtual ~Decoder_repetition_fast();

	void decode();
};

#include "Decoder_repetition_fast.hxx"

#endif /* DECODER_REPETITION_FAST */
