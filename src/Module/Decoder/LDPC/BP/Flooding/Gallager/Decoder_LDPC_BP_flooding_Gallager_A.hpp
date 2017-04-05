#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_Gallager_A : public Decoder_SISO<B,R>
{
protected:
	const int                  n_ite;           // number of iterations to perform
	const tools::AList_reader &H;               // LDPC H matrix
	const bool                 enable_syndrome; // stop criterion
	const int                  syndrome_depth;
	mipp::vector<char>         HY_N;            // input LLRs (transformed in bit)
	mipp::vector<char>         C_to_V_messages; // check    nodes to variable nodes messages
	mipp::vector<char>         V_to_C_messages; // variable nodes to check    nodes messages

public:
	Decoder_LDPC_BP_flooding_Gallager_A(const int &K, const int &N, const int& n_ite, const tools::AList_reader &H,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1,
	                                    const int n_frames = 1,
	                                    const std::string name = "Decoder_LDPC_BP_flooding_Gallager_A");
	virtual ~Decoder_LDPC_BP_flooding_Gallager_A();

protected:
	// hard decoder (load -> decode -> store)
	void _hard_decode_fbf(const R *Y_N, B *V_K);
};
}
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_ */
