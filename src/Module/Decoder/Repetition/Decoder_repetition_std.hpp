#ifndef DECODER_REPETITION_STD
#define DECODER_REPETITION_STD

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Decoder_repetition.hpp"

template <typename B, typename R>
class Decoder_repetition_std : public Decoder_repetition<B,R>
{
public:
	Decoder_repetition_std(const int& K, const int& N, const bool buffered_encoding = true, 
	                       const int n_frames = 1, const std::string name = "Decoder_repetition_std");
	virtual ~Decoder_repetition_std();

	void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);
};

#endif /* DECODER_REPETITION_STD */
