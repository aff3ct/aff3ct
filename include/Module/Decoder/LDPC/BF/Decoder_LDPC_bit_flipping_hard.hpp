/*!
 * \file
 * \brief Class module::Decoder_LDPC_bit_flipping_hard.
 */
#ifndef DECODER_LDPC_BIT_FLIPPING_HARD_HPP_
#define DECODER_LDPC_BIT_FLIPPING_HARD_HPP_

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping_hard : public Decoder_SIHO<B,R>
{
protected:
	const int  n_ite; // number of iterations to perform
	const bool enable_syndrome;
	const int  syndrome_depth;
	      int  cur_syndrome_depth;

	const tools::Sparse_matrix &H;

	// data structures for iterative decoding
	std::vector<B> var_nodes;
	std::vector<B> check_nodes;
	std::vector<B> YH_N;

	const std::vector<unsigned> &info_bits_pos;

public:
	Decoder_LDPC_bit_flipping_hard(const int &K, const int &N, const int& n_ite,
	                               const tools::Sparse_matrix &H,
	                               const std::vector<unsigned> &info_bits_pos,
	                               const bool enable_syndrome = true,
	                               const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_bit_flipping_hard() = default;
	virtual Decoder_LDPC_bit_flipping_hard<B,R>* clone() const;

protected:
	void _store        (B *V_K,                            const size_t frame_id);
	void _store_cw     (B *V_N,                            const size_t frame_id);

	int _decode_siho   (const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);

	int _decode_hiho   (const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);

	// return true if the syndrome is valid
	virtual bool     decode(const B *Y_N,                     const size_t frame_id);
	virtual void cn_process(const B *VN,  B *CN,              const size_t frame_id) = 0;
	virtual void vn_process(const B *Y_N, B *VN, const B *CN, const size_t frame_id) = 0;

	bool check_syndrome() const;
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_HARD_HPP_ */
