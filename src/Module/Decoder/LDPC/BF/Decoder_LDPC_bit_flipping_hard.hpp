#ifndef DECODER_LDPC_BIT_FLIPPING_HARD_HPP_
#define DECODER_LDPC_BIT_FLIPPING_HARD_HPP_

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping_hard : public Decoder_SIHO_HIHO<B,R>
{
public:
	Decoder_LDPC_bit_flipping_hard(const int &K, const int &N, const int& n_ite,
	                               const tools::Sparse_matrix &H,
	                               const std::vector<unsigned> &info_bits_pos,
	                               const bool enable_syndrome = true,
	                               const int syndrome_depth = 1,
	                               const int n_frames = 1);
	virtual ~Decoder_LDPC_bit_flipping_hard() = default;

protected:
	const int  n_ite;      // number of iterations to perform
	const bool enable_syndrome;
	const int  syndrome_depth;
	int cur_syndrome_depth;

	const tools::Sparse_matrix  H; // In vertical way
	                               // CN are along the columns -> H.get_n_cols() == M (often M=N-K)
	                               // VN are along the rows    -> H.get_n_rows() == N
	                               // automatically transpose in the constructor if needed

	// data structures for iterative decoding
	std::vector<B> var_nodes;
	std::vector<B> check_nodes;
	std::vector<B> YH_N;

	const std::vector<unsigned> &info_bits_pos;


	void _store         (B *V_K,               const int frame_id);
	void _store_cw      (B *V_N,               const int frame_id);

	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);

	void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);

	// return true if the syndrome is valid
	virtual bool     decode(const B *Y_N,                     const int frame_id);
	virtual void cn_process(const B *VN,  B *CN,              const int frame_id) = 0;
	virtual void vn_process(const B *Y_N, B *VN, const B *CN, const int frame_id) = 0;

	bool check_syndrome() const;
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_HARD_HPP_ */
