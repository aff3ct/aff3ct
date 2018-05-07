#ifndef DECODER_LDPC_BP_LAYERED_HPP_
#define DECODER_LDPC_BP_LAYERED_HPP_

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"

#include "../Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_layered : public Decoder_LDPC_BP<B,R>
{
protected:
	const int n_C_nodes; // number of check nodes (= N - K)

	// reset so C_to_V and V_to_C structures can be cleared only at the beginning of the loop in iterative decoding
	bool init_flag;

	const std::vector<unsigned> &info_bits_pos;

	// data structures for iterative decoding
	std::vector<std::vector<R>> var_nodes;
	std::vector<std::vector<R>> branches;
	std::vector<R> contributions;

	Update_rule rule;

public:
	Decoder_LDPC_BP_layered(const int K, const int N, const int n_ite,
	                        const tools::Sparse_matrix &H,
	                        const std::vector<unsigned> &info_bits_pos,
	                        const Update_rule &rule,
	                        const bool enable_syndrome = true,
	                        const int syndrome_depth = 1,
	                        const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_layered();

	void reset();

protected:
	void _load          (const R *Y_N,           const int frame_id);
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	// BP functions for decoding
	void BP_decode(const int frame_id);

	void BP_process(std::vector<R> &var_nodes, std::vector<R> &branches);
};
}
}

#include "Decoder_LDPC_BP_layered.hxx"

#endif /* DECODER_LDPC_BP_LAYERED_HPP_ */
