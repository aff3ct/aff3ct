#ifndef DECODER_LDPC_BP_NAIVE_HPP_
#define DECODER_LDPC_BP_NAIVE_HPP_

#include "../../Decoder.hpp"
#include "../../SISO.hpp"

#define MESSAGE 7392

template <typename B, typename R>
class Decoder_LDPC_BP_naive : public Decoder<B,R>, public SISO<R>
{
private:
	mipp::vector<R> Y_N;
	mipp::vector<R> Y_Nbis;
	mipp::vector<B> V_K;
	const int n_ite;

	// data structures for iterative decoding */
	mipp::vector<R> APPllr;
	mipp::vector<R> CtoV;
	mipp::vector<R> VtoC;

	// reset so CtoV and VtoC structures can be cleared only at the begining of the loop in iterative decoding
	int iter_cnt;
	int ldpc_iter; // used for debug

	// BP constants for decoding
	const int ldpc_nb_iter_max  = 1;
	const int fast_stop         = 0;
	const int C2V_max           = 15;
	const int VNodesNb          = 4224;
	const int CNodesNb          = 2112;
	const int message           = MESSAGE;

	// noms tuning
	const float offset      = 0.000; // 0.00 // 0.15
	const float normalize_1 = 0.825; // 1.00 // 1.25 // 0.825
	const float normalize_2 = 0.825; // 1.00 // 1.25 // 0.825
	
	// prerequisite functions for decoding
	R    min_2   (const R a, const R min1, const R min2);
	int  signbit (const R val                          );
	void saturate(mipp::vector<R> &array, R saturation );

	// BP functions for decoding
	bool BP_decode(const R                var_nodeS[],  
	               const int              nombre_iterations,  
	               const int              initFlag, 
	                     mipp::vector<R> &C2V,
	                     mipp::vector<R> &V2C,
	                     mipp::vector<R> &Rprime_fix,
	                     int*             iters);

	bool SPA_decode(const R                var_nodeS[],  
	                const int              nombre_iterations,  
	                const int              initFlag, 
	                      mipp::vector<R> &C2V,
	                      mipp::vector<R> &V2C,
	                      mipp::vector<R> &Rprime_fix,
	                      int*             iters);

	bool LogSPA_decode(const R var_nodeS[],  
	                   const int nombre_iterations,
	                   const int initFlag, 
	                   mipp::vector<R> &C2V,
	                   mipp::vector<R> &V2C,
	                   mipp::vector<R> &Rprime_fix,
	                   int* iters);

public:
	Decoder_LDPC_BP_naive(const int &N, const int &K, const int& n_ite);
	virtual ~Decoder_LDPC_BP_naive();

	// unsupported prototype
	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

	// soft decode
	void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

	// hard decoder (load -> decode -> store)
	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;
};

#endif /* DECODER_LDPC_BP_NAIVE_HPP_ */