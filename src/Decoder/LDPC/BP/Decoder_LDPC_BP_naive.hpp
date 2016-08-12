#ifndef DECODER_LDPC_BP_NAIVE_HPP_
#define DECODER_LDPC_BP_NAIVE_HPP_

#include "../../Decoder.hpp"
#include "../../SISO.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_naive : public Decoder<B,R>, public SISO<R>
{
private:
	// BP constants for decoding
	const int n_ite;
	const int fast_stop      = 0;
	const int C_to_V_max     = 15;
	const int n_V_nodes      = 4224;
	const int n_C_nodes      = 2112;
	const int message_length = 7392;

	// noms tuning
	const float offset     = 0.000; // 0.00 // 0.15
	const float normalize1 = 0.825; // 1.00 // 1.25 // 0.825
	const float normalize2 = 0.825; // 1.00 // 1.25 // 0.825

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	int init_flag;
	
	mipp::vector<R> Y_N;
	mipp::vector<B> V_K;

	// data structures for iterative decoding */
	mipp::vector<R> Lp_N;
	mipp::vector<R> C_to_V;
	mipp::vector<R> V_to_C;

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

private:
	// prerequisite functions for decoding
	int  signbit (const R val                         );
	void saturate(mipp::vector<R> &array, R saturation);

	// BP functions for decoding
	bool BP_decode(const mipp::vector<R> &var_nodes);

	// // dead code
	// bool SPA_decode(const R                var_nodeS[],  
	//                 const int              nombre_iterations,  
	//                 const int              initFlag, 
	//                       mipp::vector<R> &C2V,
	//                       mipp::vector<R> &V2C,
	//                       mipp::vector<R> &Rprime_fix,
	//                       int             *iters);

	// bool LogSPA_decode(const R          var_nodeS[],  
	//                    const int        nombre_iterations,
	//                    const int        initFlag, 
	//                    mipp::vector<R> &C2V,
	//                    mipp::vector<R> &V2C,
	//                    mipp::vector<R> &Rprime_fix,
	//                    int             *iters);
};

#endif /* DECODER_LDPC_BP_NAIVE_HPP_ */