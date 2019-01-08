#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_E_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_E_HPP_

#include <cstdint>

#include "../../../../Decoder_SIHO_HIHO.hpp"
#include "../../Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_Gallager_E : public Decoder_SIHO_HIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<uint32_t> &info_bits_pos;

	std::vector<B       > HY_N;       // input LLRs (transformed in bit)
	std::vector<int8_t  > V_N;        // decoded bits
	std::vector<int8_t  > chk_to_var; // check    nodes to variable nodes messages
	std::vector<int8_t  > var_to_chk; // variable nodes to check    nodes messages
	std::vector<unsigned> transpose;

	int scaling;

public:
	Decoder_LDPC_BP_flooding_Gallager_E(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
	                                    const std::vector<unsigned> &info_bits_pos,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1,
	                                    const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_flooding_Gallager_E() = default;

protected:
	void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N, B *V_K, const int frame_id);
	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_K, const int frame_id);

	void _decode               (const B *Y_N);
	void _initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var, std::vector<int8_t> &var_to_chk,
	                            const bool first_ite);
	void _decode_single_ite    (const std::vector<int8_t> &var_to_chk, std::vector<int8_t> &chk_to_var);
	void _make_majority_vote   (const B *Y_N, std::vector<int8_t> &V_N);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_BP_flooding_GALE = Decoder_LDPC_BP_flooding_Gallager_E<B,R>;
}
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_E_HPP_ */
