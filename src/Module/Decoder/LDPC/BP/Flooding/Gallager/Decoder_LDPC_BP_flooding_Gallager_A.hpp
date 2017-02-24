#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
template <typename B, typename R>
class Decoder_LDPC_BP_flooding_Gallager_A : public Decoder_SISO<B,R>
{
protected:
	const int           n_ite;           // number of iterations to perform
	const AList_reader &H;               // LDPC H matrix
	const bool          enable_syndrome; // stop criterion
	mipp::vector<char>  Y_N;             // input LLRs (transformed in bit)
	mipp::vector<char>  C_to_V_messages; // check    nodes to variable nodes messages
	mipp::vector<char>  V_to_C_messages; // variable nodes to check    nodes messages

public:
	Decoder_LDPC_BP_flooding_Gallager_A(const int &K, const int &N, const int& n_ite, const AList_reader &H,
	                                    const bool enable_syndrome = true,
	                                    const int n_frames = 1,
	                                    const std::string name = "Decoder_LDPC_BP_flooding_Gallager_A");
	virtual ~Decoder_LDPC_BP_flooding_Gallager_A();

protected:
	// hard decoder (load -> decode -> store)
	void load       (const mipp::vector<R>& Y_N);
	void hard_decode(                          );
	void store      (      mipp::vector<B>& V_K) const;

	void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

public:
	void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

};
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_ */
