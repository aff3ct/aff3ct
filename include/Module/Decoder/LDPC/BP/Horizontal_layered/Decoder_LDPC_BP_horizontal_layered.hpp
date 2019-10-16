#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_HPP_
#define DECODER_LDPC_BP_HORIZONTAL_LAYERED_HPP_

#include <cstdint>
#include <vector>

#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_horizontal_layered : public Decoder_SISO_SIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<uint32_t> &info_bits_pos;

	Update_rule up_rule;

	// data structures for iterative decoding
	std::vector<std::vector<R>> var_nodes;
	std::vector<std::vector<R>> messages;
	std::vector<R             > contributions;

	bool init_flag; // reset the chk_to_var vector at the begining of the iterative decoding

public:
	Decoder_LDPC_BP_horizontal_layered(const int K, const int N, const int n_ite,
	                                   const tools::Sparse_matrix &H,
	                                   const std::vector<unsigned> &info_bits_pos,
	                                   const Update_rule &up_rule,
	                                   const bool enable_syndrome = true,
	                                   const int syndrome_depth = 1,
	                                   const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_horizontal_layered() = default;
	void reset();

protected:
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	void _load             (const R *Y_N, const int frame_id);
	void _decode           (const int frame_id);
	void _decode_single_ite(std::vector<R> &var_nodes, std::vector<R> &messages);
};
}
}

#include "Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered.hxx"

#endif /* DECODER_LDPC_BP_HORIZONTAL_LAYERED_HPP_ */
