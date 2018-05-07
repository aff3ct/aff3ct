#ifndef DECODER_LDPC_BP_FLOODING_HPP_
#define DECODER_LDPC_BP_FLOODING_HPP_

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"

#include "../Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_flooding : public Decoder_LDPC_BP<B,R>
{
public:
	void reset();

protected:
	const int  n_V_nodes;  // number of variable nodes (= N)
	const int  n_C_nodes;  // number of check    nodes (= N - K)
	const int  n_branches; // number of branched in the bi-partite graph (connexions between the V and C nodes)

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	bool init_flag;

	const std::vector<uint32_t> &info_bits_pos;

	std::vector<uint8_t > n_variables_per_parity;
	std::vector<uint8_t > n_parities_per_variable;
	std::vector<uint32_t> transpose;

	// data structures for iterative decoding
	            std::vector<R>  Lp_N;   // a posteriori information
	std::vector<std::vector<R>> C_to_V; // check    nodes to variable nodes messages
	std::vector<std::vector<R>> V_to_C; // variable nodes to check    nodes messages

	Update_rule rule;

public:
	Decoder_LDPC_BP_flooding(const int K, const int N, const int n_ite,
	                         const tools::Sparse_matrix &H,
	                         const std::vector<uint32_t> &info_bits_pos,
	                         const Update_rule &rule,
	                         const bool enable_syndrome = true,
	                         const int syndrome_depth = 1,
	                         const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_flooding();

	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	// BP functions for decoding
	void BP_decode(const R *Y_N, const int frame_id);

	void BP_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V);
};
}
}

#include "Decoder_LDPC_BP_flooding.hxx"

#endif /* DECODER_LDPC_BP_FLOODING_HPP_ */
