#ifndef DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_
#define DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_

#include <cstdint>

#include "Module/Decoder/NO/Decoder_NO.hpp"

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_flooding_Gallager_A : public Decoder_LDPC_BP<B,R>
{
private:
	Decoder_NO<B,R> hard_decision;

protected:
	const std::vector<unsigned> &info_bits_pos;
	std::vector<B>               HY_N;            // input LLRs (transformed in bit)
	std::vector<int8_t>          V_N;             // decoded bits
	std::vector<int8_t>          C_to_V_messages; // check    nodes to variable nodes messages
	std::vector<int8_t>          V_to_C_messages; // variable nodes to check    nodes messages
	std::vector<unsigned>        transpose;

public:
	Decoder_LDPC_BP_flooding_Gallager_A(const int K, const int N, const int n_ite, const tools::Sparse_matrix &H,
	                                    const std::vector<unsigned> &info_bits_pos,
	                                    const bool enable_syndrome = true,
	                                    const int syndrome_depth = 1,
	                                    const int n_frames = 1);
	virtual ~Decoder_LDPC_BP_flooding_Gallager_A();

protected:
	void _decode        (const B *Y_N                            );
	void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N, B *V_K, const int frame_id);
	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_K, const int frame_id);
};

template <typename B = int, typename R = float>
using Decoder_LDPC_BP_flooding_GALA = Decoder_LDPC_BP_flooding_Gallager_A<B,R>;
}
}

#endif /* DECODER_LDPC_BP_FLOODING_GALLAGER_A_HPP_ */
