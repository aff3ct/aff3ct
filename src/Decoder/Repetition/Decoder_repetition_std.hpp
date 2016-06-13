#ifndef DECODER_REPETITION_STD
#define DECODER_REPETITION_STD

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "Decoder_repetition.hpp"

template <typename B, typename R>
class Decoder_repetition_std : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_std(const int& K, const int& N);
	virtual ~Decoder_repetition_std();

	void decode();
};

#include "Decoder_repetition_std.hxx"

#endif /* DECODER_REPETITION_STD */
