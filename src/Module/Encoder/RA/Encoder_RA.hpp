#ifndef ENCODER_RA_HPP_
#define ENCODER_RA_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"

namespace aff3ct
{
template <typename B>
class Encoder_RA : public Encoder<B>
{
protected:
	const int rep_count; // number of repetitions
	mipp::vector<B> U, tmp_X_N;

	Interleaver<int>& interleaver;

public:
    Encoder_RA(const int& K, const int& N, Interleaver<int>& interleaver, const std::string name = "Encoder_RA");

	virtual ~Encoder_RA() {}

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};
}

#endif // ENCODER_RA_HPP_
