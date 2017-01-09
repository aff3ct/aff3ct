#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_NAIVE_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_A_NAIVE_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../../../Decoder_SISO.hpp"

template <typename B, typename R>
class Decoder_LDPC_BP_flooding_Gallager_A_naive : public Decoder_SISO<B,R>
{
protected:
	const int                          n_ite;     // number of iterations to perform
	const AList_reader                &H;         // LDPC H matrix
	const mipp::vector<unsigned int >  transpose;

	      mipp::vector<unsigned char>  Y_N;       // input LLRs (transformed in bit)
	      mipp::vector<unsigned char>  C_to_V;    // check    nodes to variable nodes messages
	      mipp::vector<unsigned char>  V_to_C;    // variable nodes to check    nodes messages

public:
	Decoder_LDPC_BP_flooding_Gallager_A_naive(const int &K, const int &N, const int& n_ite, const AList_reader &H,
	                                          const int n_frames = 1,
	                                          const std::string name = "Decoder_LDPC_BP_flooding_Gallager_A_naive");
	virtual ~Decoder_LDPC_BP_flooding_Gallager_A_naive();

protected:
	// hard decoder (load -> decode -> store)
	void load       (const mipp::vector<R>& Y_N);
	void hard_decode(                          );
	void store      (      mipp::vector<B>& V_K) const;

	void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

public:
	void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

};

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_A_NAIVE_HPP_ */
