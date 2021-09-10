/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_horizontal_layered_ONMS_inter.
 */
#ifndef DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_
#define DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_

#include <vector>
#include <mipp.h>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_horizontal_layered_ONMS_inter : public Decoder_SISO<B,R>, public Decoder_LDPC_BP
{
private:
	const float normalize_factor;
	const R offset;
	mipp::vector<mipp::Reg<R>> contributions;

protected:
	const R saturation;

	const std::vector<unsigned> info_bits_pos;

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
	                                              const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_horizontal_layered_ONMS_inter() = default;
	virtual Decoder_LDPC_BP_horizontal_layered_ONMS_inter<B,R>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

protected:
	void _reset(const size_t frame_id);

	int _decode_siso   (const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id);
	int _decode_siho   (const R *Y_N,  int8_t *CWD, B *V_K,  const size_t frame_id);
	int _decode_siho_cw(const R *Y_N,  int8_t *CWD, B *V_N,  const size_t frame_id);

	void _load(const R *Y_N, const size_t frame_id);
	template <int F = 1>
	int _decode(const size_t frame_id);
	template <int F = 1>
	void _decode_single_ite(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &branches);
	bool _check_syndrome(const size_t frame_id);
	int  _check_syndrome_status(const size_t frame_id);
};
}
}

#endif /* DECODER_LDPC_BP_HORIZONTAL_LAYERED_ONMS_INTER_HPP_ */
