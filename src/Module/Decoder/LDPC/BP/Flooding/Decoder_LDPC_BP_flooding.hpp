#ifndef DECODER_LDPC_BP_FLOODING_HPP_
#define DECODER_LDPC_BP_FLOODING_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../../../Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding : public Decoder_SISO<B,R>
{
protected:
	const int  n_ite;      // number of iterations to perform
	const int  n_V_nodes;  // number of variable nodes (= N)
	const int  n_C_nodes;  // number of check    nodes (= N - K)
	const int  n_branches; // number of branched in the bi-partite graph (connexions between the V and C nodes)

	const bool enable_syndrome;
	const int  syndrome_depth;

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	bool init_flag;

	const mipp::vector<B> &info_bits_pos;

	const mipp::vector<unsigned char> n_variables_per_parity;
	const mipp::vector<unsigned char> n_parities_per_variable;
	const mipp::vector<unsigned int > transpose;

	// data structures for iterative decoding
	            mipp::vector<R>  Lp_N;   // a posteriori information
	std::vector<mipp::vector<R>> C_to_V; // check    nodes to variable nodes messages
	std::vector<mipp::vector<R>> V_to_C; // variable nodes to check    nodes messages

	Decoder_LDPC_BP_flooding(const int &K, const int &N, const int& n_ite, 
	                         const tools::AList_reader &alist_data,
	                         const mipp::vector<B> &info_bits_pos,
	                         const bool enable_syndrome = true,
	                         const int syndrome_depth = 1,
	                         const int n_frames = 1,
	                         const std::string name = "Decoder_LDPC_BP_flooding");
	virtual ~Decoder_LDPC_BP_flooding();

	// soft decode
	void _soft_decode(const R *Y_N1, R *Y_N2, const int frame_id);

	// hard decoder
	void _hard_decode(const R *Y_N, B *V_K, const int frame_id);

	// BP functions for decoding
	void BP_decode(const R *Y_N, const int frame_id);

	virtual bool BP_process(const R *Y_N, mipp::vector<R> &V_to_C, mipp::vector<R> &C_to_V) = 0;
};
}
}

#endif /* DECODER_LDPC_BP_FLOODING_HPP_ */
