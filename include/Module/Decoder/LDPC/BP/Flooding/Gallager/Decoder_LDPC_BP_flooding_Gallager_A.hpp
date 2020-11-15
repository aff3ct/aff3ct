/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_flooding_Gallager_A.
 */
#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_

#include <cstdint>
#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_Gallager_A : public Decoder_SIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const bool transform_HY_N;
	const std::vector<uint32_t> &info_bits_pos;

	std::vector<B       > HY_N;       // input LLRs (transformed in bit)
	std::vector<int8_t  > V_N;        // decoded bits
	std::vector<int8_t  > chk_to_var; // check    nodes to variable nodes messages
	std::vector<int8_t  > var_to_chk; // variable nodes to check    nodes messages
	std::vector<unsigned> transpose;

protected:
	Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
	                                    const std::vector<unsigned> &info_bits_pos,
	                                    const bool transform_HY_N,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1);

public:
	Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
	                                    const std::vector<unsigned> &info_bits_pos,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_flooding_Gallager_A() = default;
	virtual Decoder_LDPC_BP_flooding_Gallager_A<B,R>* clone() const;

protected:
	int _decode_hiho   (const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_siho   (const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode        (const B *Y_N);

	virtual void _initialize_var_to_chk(const B *Y_N, const std::vector<int8_t> &chk_to_var,
	                                    std::vector<int8_t> &var_to_chk, const int ite);
	virtual void _decode_single_ite    (const std::vector<int8_t> &var_to_chk, std::vector<int8_t> &chk_to_var);
	virtual void _make_majority_vote   (const B *Y_N, std::vector<int8_t> &V_N);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_BP_flooding_GALA = Decoder_LDPC_BP_flooding_Gallager_A<B,R>;
}
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_ */
