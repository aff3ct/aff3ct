/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_vertical_layered.
 */
#ifndef DECODER_LDPC_BP_VERTICAL_LAYERED_HPP_
#define DECODER_LDPC_BP_VERTICAL_LAYERED_HPP_

#include <cstdint>
#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Update_rule/SPA/Update_rule_SPA.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, class Update_rule = tools::Update_rule_SPA<R>>
class Decoder_LDPC_BP_vertical_layered : public Decoder_SISO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<uint32_t> info_bits_pos;

	Update_rule up_rule;

	// data structures for iterative decoding
	std::vector<std::vector<R>> var_nodes;
	std::vector<std::vector<R>> messages;
	std::vector<R             > contributions;
	std::vector<uint32_t      > messages_offsets;

public:
	Decoder_LDPC_BP_vertical_layered(const int K, const int N, const int n_ite,
	                                 const tools::Sparse_matrix &H,
	                                 const std::vector<unsigned> &info_bits_pos,
	                                 const Update_rule &up_rule,
	                                 const bool enable_syndrome = true,
	                                 const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_vertical_layered() = default;

	virtual Decoder_LDPC_BP_vertical_layered<B,R,Update_rule>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	void _reset(const size_t frame_id);

	int _decode_siso   (const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id);
	int _decode_siho   (const R *Y_N,  int8_t *CWD, B *V_K,  const size_t frame_id);
	int _decode_siho_cw(const R *Y_N,  int8_t *CWD, B *V_N,  const size_t frame_id);

	void _load             (const R *Y_N, const size_t frame_id);
	int  _decode           (const size_t frame_id);
	void _decode_single_ite(std::vector<R> &var_nodes, std::vector<R> &messages);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Decoder/LDPC/BP/Vertical_layered/Decoder_LDPC_BP_vertical_layered.hxx"
#endif

#endif /* DECODER_LDPC_BP_VERTICAL_LAYERED_HPP_ */
