#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>
#include "../../Tools/mipp.h"

#include "../Decoder.hpp"
#include "../../Tools/Algebraic/Galois.hpp"

template <typename B, typename R>
class Decoder_BCH : public Decoder<B, R>
{

private:
	mipp::vector<mipp::vector<int>> elp;
	mipp::vector<int> discrepancy;
	mipp::vector<int> l;
	mipp::vector<int> u_lu;
	mipp::vector<int> s;
	mipp::vector<int> loc;
	mipp::vector<int> reg;
	
protected:
	
    const int K;				// info bits
    const int N;				// code length
	const int m;				// order of the Galois Field
	const int t;				// correction power
	const int d;				// minimum distance of the code (d=2t+1))
	
	mipp::vector<int> alpha_to;	// log table of GF(2**m)
	mipp::vector<int> index_of;	// antilog table of GF(2**m)

	mipp::vector<B> YH_N;		// hard decision input vector
	mipp::vector<R> Y_N;
    mipp::vector<B> V_K;
	        
public:
    Decoder_BCH(const int& K, const int& N, const int& m, const int&t, Galois* GF);
    virtual ~Decoder_BCH();

    void load  (const mipp::vector<R>& Y_N);
    void decode(                          );
    void store (      mipp::vector<B>& V_K) const;

};

#include "Decoder_BCH.hxx"

#endif /* DECODER_BCH */
