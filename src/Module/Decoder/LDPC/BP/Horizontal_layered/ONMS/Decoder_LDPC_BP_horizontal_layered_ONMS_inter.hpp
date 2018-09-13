#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_
#define DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_

#include <mipp.h>

#include "../../../../Decoder_SISO_SIHO.hpp"
#include "../../Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_horizontal_layered_ONMS_inter : public Decoder_SISO_SIHO<B,R>, public Decoder_LDPC_BP
{
private:
	const float normalize_factor;
	const R offset;
	mipp::vector<mipp::Reg<R>> contributions;

protected:
	const R saturation;

	// reset so C_to_V and V_to_C structures can be cleared only at the beginning of the loop in iterative decoding
	bool init_flag;

	const std::vector<unsigned> &info_bits_pos;

	// data structures for iterative decoding
	std::vector<mipp::vector<mipp::Reg<R>>> var_nodes;
	std::vector<mipp::vector<mipp::Reg<R>>> branches;

	mipp::vector<mipp::Reg<R>> Y_N_reorderered;
	mipp::vector<mipp::Reg<B>> V_reorderered;

public:
	Decoder_LDPC_BP_horizontal_layered_ONMS_inter(const int K, const int N, const int n_ite,
	                                              const tools::Sparse_matrix &H,
	                                              const std::vector<unsigned> &info_bits_pos,
	                                              const float normalize_factor = 1.f,
	                                              const R offset = (R)0,
	                                              const bool enable_syndrome = true,
	                                              const int syndrome_depth = 1,
	                                              const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_horizontal_layered_ONMS_inter() = default;

	void reset();

protected:
	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	void _load(const R *Y_N, const int frame_id);
	template <int F = 1>
	void _decode(const int frame_id);
	template <int F = 1>
	void _decode_single_ite(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &branches);
	bool _check_syndrome(const int frame_id);
};
}
}

#endif /* DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_ */
