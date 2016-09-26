#ifndef DECODER_REPETITION_FAST
#define DECODER_REPETITION_FAST

#include <vector>
#include "Tools/MIPP/mipp.h"

#include "Decoder_repetition.hpp"

template <typename B, typename R>
class Decoder_repetition_fast : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_fast(const int& K, const int& N, const bool buffered_encoding = true,
	                        const std::string name = "Decoder_repetition_fast");
	virtual ~Decoder_repetition_fast();

	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);
};

#include "Decoder_repetition_fast.hxx"

#endif /* DECODER_REPETITION_FAST */
