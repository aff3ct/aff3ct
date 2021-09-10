/*!
 * \file
 * \brief Class module::Decoder_LDPC_BP_peeling.
 */
#ifndef DECODER_LDPC_BP_PEELING_HPP
#define DECODER_LDPC_BP_PEELING_HPP

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{

template<typename B = int, typename R = float>
class Decoder_LDPC_BP_peeling : public Decoder_SIHO<B,R>, public Decoder_LDPC_BP
{
protected:
	const std::vector<unsigned> info_bits_pos;

	// data structures for iterative decoding
	std::vector<B> var_nodes;
	std::vector<B> check_nodes;

public:
	Decoder_LDPC_BP_peeling(const int K, const int N, const int n_ite,
	                        const tools::Sparse_matrix &H,
	                        const std::vector<unsigned> &info_bits_pos,
	                        const bool enable_syndrome = true,
	                        const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_BP_peeling() = default;
	virtual Decoder_LDPC_BP_peeling<B,R>* clone() const;

protected:
	void _store         (B *V_K,                            const size_t frame_id);
	void _store_cw      (B *V_N,                            const size_t frame_id);

	int _decode_hiho    (const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_hiho_cw (const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	int _decode_siho    (const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_siho_cw (const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	// return true if the syndrome is valid
	virtual bool _decode(                                   const size_t frame_id);
};

}
}

#endif //DECODER_LDPC_BP_PEELING_HPP
