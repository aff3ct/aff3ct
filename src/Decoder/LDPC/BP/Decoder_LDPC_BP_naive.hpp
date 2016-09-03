#ifndef DECODER_LDPC_BP_NAIVE_HPP_
#define DECODER_LDPC_BP_NAIVE_HPP_

#include "../../Decoder_SISO.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_naive : public Decoder_SISO<B,R>
{
protected:
	const int n_ite;      // number of iterations to perform
	const int n_V_nodes;  // number of variable nodes (= N)
	const int n_C_nodes;  // number of check    nodes (= N - K)
	const int n_branches; // number of branched in the bi-partite graph (connexions between the V and C nodes)

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	bool init_flag;

	const std::vector<unsigned char> &n_variables_per_parity;
	const std::vector<unsigned char> &n_parities_per_variable;
	const std::vector<unsigned int > &transpose;
	
	mipp::vector<R> Y_N; // input  LLRs
	mipp::vector<B> V_K; // output bits

	// data structures for iterative decoding
	mipp::vector<R> Lp_N;   // a posteriori information
	mipp::vector<R> C_to_V; // check    nodes to variable nodes messages
	mipp::vector<R> V_to_C; // variable nodes to check    nodes messages

public:
	Decoder_LDPC_BP_naive(const int &K, const int &N, const int& n_ite, 
	                      const std::vector<unsigned char> &n_variables_per_parity,
	                      const std::vector<unsigned char> &n_parities_per_variable,
	                      const std::vector<unsigned int > &transpose,
	                      const std::string name = "Decoder_LDPC_BP_naive");
	virtual ~Decoder_LDPC_BP_naive();

	// unsupported prototype
	void decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

	// soft decode
	void decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

	// hard decoder (load -> decode -> store)
	void load  (const mipp::vector<R>& Y_N);
	void decode(                          );
	void store (      mipp::vector<B>& V_K) const;

protected:
	// BP functions for decoding
	bool BP_decode(const mipp::vector<R> &Y_N);

	virtual bool BP_process() = 0;
};

#endif /* DECODER_LDPC_BP_NAIVE_HPP_ */