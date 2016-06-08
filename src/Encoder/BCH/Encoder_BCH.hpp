#ifndef ENCODER_BCH_HPP_
#define ENCODER_BCH_HPP_

#include <vector>
#include "../../Tools/mipp.h"

#include "../Encoder.hpp"
#include "../../Tools/Algebraic/Galois.hpp"


template <typename B>
class Encoder_BCH : public Encoder<B>
{
	
protected:

    	
    const int K;         // info bits
    const int N;         // code length
	const int m;         // order of the Galois Field
	
	mipp::vector<int> g;           // coefficients of the generator polynomial, g(x)
	mipp::vector<int> bb;          // coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)

public:
    Encoder_BCH(const int& K, const int& N, const int& m, Galois* GF);

    virtual ~Encoder_BCH() {}

    virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#include "Encoder_BCH.hxx"

#endif // ENCODER_BCH_HPP_
