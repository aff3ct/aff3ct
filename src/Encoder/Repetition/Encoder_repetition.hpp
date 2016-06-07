#ifndef ENCODER_REPETITION_HPP_
#define ENCODER_REPETITION_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Encoder.hpp"

template <typename B>
class Encoder_repetition : public Encoder<B>
{
protected:
	const int K;         // info bits
	const int N;         // code length
	const int rep_count; // number of repetition
	
public:
	Encoder_repetition(const int& K, const int& N);
	virtual ~Encoder_repetition() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#endif // ENCODER_REPETITION_HPP_
