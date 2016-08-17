#ifndef ENCODER_RA_HPP_
#define ENCODER_RA_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Encoder.hpp"
#include "../../Interleaver/Interleaver.hpp"

template <typename B>
class Encoder_RA : public Encoder<B>
{
protected:
    const int K;         // info bits
    const int N;         // code length
    const int rep_count; // number of repetitions
    mipp::vector<B> U;

    Interleaver<int>& interleaver;

public:
    Encoder_RA(const int& K, const int& N, Interleaver<int>& interleaver, const std::string name = "Encoder_RA");

    virtual ~Encoder_RA() {}

    virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#endif // ENCODER_RA_HPP_
