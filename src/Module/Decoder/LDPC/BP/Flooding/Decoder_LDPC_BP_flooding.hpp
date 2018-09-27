#ifndef DECODER_LDPC_BP_FLOODING_HPP_
#define DECODER_LDPC_BP_FLOODING_HPP_

#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"

#include "../../../Decoder_SISO_SIHO.hpp"
#include "../Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_flooding : public Decoder_SISO_SIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<uint32_t> &info_bits_pos;

	Update_rule up_rule;

	std::vector<uint32_t      > transpose;
	std::vector<R             > post;       // a posteriori information
	std::vector<std::vector<R>> msg_chk_to_var; // check    nodes to variable nodes messages
	std::vector<std::vector<R>> msg_var_to_chk; // variable nodes to check    nodes messages

	bool init_flag; // reset the msg_chk_to_var vector at the begining of the iterative decoding

public:
	Decoder_LDPC_BP_flooding(const int K, const int N, const int n_ite,
	                         const tools::Sparse_matrix &H,
	                         const std::vector<uint32_t> &info_bits_pos,
	                         const Update_rule &up_rule,
	                         const bool enable_syndrome = true,
	                         const int syndrome_depth = 1,
	                         const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_flooding() = default;
	void reset();

protected:
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	void _decode               (const R *Y_N, const int frame_id);
	void _initialize_var_to_chk(const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &msg_var_to_chk);
	void _decode_single_ite    (              const std::vector<R> &msg_var_to_chk, std::vector<R> &msg_chk_to_var);
	void _compute_post         (const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &post);
};
}
}

#include "Decoder_LDPC_BP_flooding.hxx"

#endif /* DECODER_LDPC_BP_FLOODING_HPP_ */