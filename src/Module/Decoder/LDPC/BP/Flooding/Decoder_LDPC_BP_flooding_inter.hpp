#ifndef DECODER_LDPC_BP_FLOODING_INTER_HPP_
#ifdef __cpp_aligned_new
#define DECODER_LDPC_BP_FLOODING_INTER_HPP_

#include <cstdint>
#include <vector>
#include <mipp.h>

#include "Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS_simd.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_NMS_simd<R>>
class Decoder_LDPC_BP_flooding_inter : public Decoder_SISO_SIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<unsigned> &info_bits_pos;

	Update_rule up_rule;

	const R sat_val;

	std::vector<uint32_t> transpose;

	mipp::vector<mipp::Reg<R>>              post;       // a posteriori information
	std::vector<mipp::vector<mipp::Reg<R>>> msg_chk_to_var; // check    nodes to variable nodes messages
	std::vector<mipp::vector<mipp::Reg<R>>> msg_var_to_chk; // variable nodes to check    nodes messages

	mipp::vector<mipp::Reg<R>> Y_N_reorderered;
	mipp::vector<mipp::Reg<B>> V_reorderered;

	bool init_flag;

public:
	Decoder_LDPC_BP_flooding_inter(const int K, const int N, const int n_ite,
	                               const tools::Sparse_matrix &H,
	                               const std::vector<unsigned> &info_bits_pos,
	                               const Update_rule &up_rule,
	                               const bool enable_syndrome = true,
	                               const int syndrome_depth = 1,
	                               const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_flooding_inter() = default;
	void reset();

protected:
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	void _load                 (const R *Y_N, const int frame_id);
	void _decode               (const mipp::Reg<R> *Y_N, const int cur_wave);
	void _initialize_var_to_chk(const mipp::Reg<R> *Y_N, const mipp::vector<mipp::Reg<R>> &msg_chk_to_var,
	                                                           mipp::vector<mipp::Reg<R>> &msg_var_to_chk);
	void _decode_single_ite    (const mipp::vector<mipp::Reg<R>> &msg_var_to_chk,
		                              mipp::vector<mipp::Reg<R>> &msg_chk_to_var);
	void _compute_post         (const mipp::Reg<R> *Y_N, const mipp::vector<mipp::Reg<R>> &msg_chk_to_var,
	                                                           mipp::vector<mipp::Reg<R>> &post);
	bool _check_syndrome_soft  (const mipp::vector<mipp::Reg<R>> &var_nodes);
};
}
}

#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding_inter.hxx"

#endif
#endif /* DECODER_LDPC_BP_FLOODING_INTER_HPP_ */