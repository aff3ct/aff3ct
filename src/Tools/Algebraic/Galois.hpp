#ifndef GALOIS
#define GALOIS

#include <vector>
#include "../../Tools/mipp.h"

class Galois
{
	
protected:
	
    const int m;					// order of the Galois Field
    const int K;					
	const int N;					// number of non-nul elements in the field : N = 2^m - 1
	const int t;
	const int d;
	        
public:
    Galois(const int& m, const int& N, const int& K, const int& t);
    virtual ~Galois();

	void Select_Polynomial(); // move this private section
	void Generate_GF();
	void Compute_BCH_Generator_Polynomial();

    mipp::vector<int> alpha_to;		// log table of GF(2**m)
	mipp::vector<int> index_of;		// antilog table of GF(2**m)
	mipp::vector<int> p;			// coefficients of a primitive polynomial used to generate GF(2**m)
	mipp::vector<int> g;			// coefficients of the generator polynomial, g(x)
};

#include "Galois.hxx"

#endif /* GALOIS */