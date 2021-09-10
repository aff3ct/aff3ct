/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_flooding.
 */
#ifndef DECODER_LDPC_BP_FLOODING_HPP_
#define DECODER_LDPC_BP_FLOODING_HPP_

#include <vector>
#include <cstdint>

#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"
#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_flooding : public Decoder_SISO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<uint32_t> info_bits_pos;

	Update_rule up_rule;

	std::vector<uint32_t      > transpose;
	std::vector<R             > post;       // a posteriori information
	std::vector<std::vector<R>> msg_chk_to_var; // check    nodes to variable nodes messages
	std::vector<std::vector<R>> msg_var_to_chk; // variable nodes to check    nodes messages

public:
	Decoder_LDPC_BP_flooding(const int K, const int N, const int n_ite,
	                         const tools::Sparse_matrix &H,
	                         const std::vector<uint32_t> &info_bits_pos,
	                         const Update_rule &up_rule,
	                         const bool enable_syndrome = true,
	                         const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_flooding() = default;

	virtual Decoder_LDPC_BP_flooding<B,R,Update_rule>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	void _reset(const size_t frame_id);

	int _decode_siso   (const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id);
	int _decode_siho   (const R *Y_N,  int8_t *CWD, B *V_K,  const size_t frame_id);
	int _decode_siho_cw(const R *Y_N,  int8_t *CWD, B *V_N,  const size_t frame_id);

	        int  _decode               (const R *Y_N, const size_t frame_id);
	        void _initialize_var_to_chk(const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &msg_var_to_chk);
	virtual void _decode_single_ite    (              const std::vector<R> &msg_var_to_chk, std::vector<R> &msg_chk_to_var);
	        void _compute_post         (const R *Y_N, const std::vector<R> &msg_chk_to_var, std::vector<R> &post);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/LDPC/BP/Flooding/Decoder_LDPC_BP_flooding.hxx"
#endif

#endif /* DECODER_LDPC_BP_FLOODING_HPP_ */