/*!
 * \file
 * \brief Class module::Decoder_LDPC_bit_flipping.
 */
#ifndef DECODER_LDPC_BIT_FLIPPING_HPP_
#define DECODER_LDPC_BIT_FLIPPING_HPP_

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping : public Decoder_SISO<B,R>
{
protected:
	const int  n_ite;      // number of iterations to perform
	const int  n_V_nodes;  // number of variable nodes (= N)
	const int  n_C_nodes;  // number of check    nodes (= N - K)
	const int  n_branches; // number of branched in the bi-partite graph (connexions between the V and C nodes)
	const R    mwbf_factor;
	const bool enable_syndrome;
	const int  syndrome_depth;

	const tools::Sparse_matrix &H;

	const std::vector<unsigned> &info_bits_pos;

	std::vector<unsigned char> n_variables_per_parity;
	std::vector<unsigned char> n_parities_per_variable;
	std::vector<unsigned int > transpose;

	// data structures for iterative decoding
	            std::vector<R>  Lp_N;   // a posteriori information
	std::vector<std::vector<R>> C_to_V; // check    nodes to variable nodes messages
	std::vector<std::vector<R>> V_to_C; // variable nodes to check    nodes messages

	std::vector<R> Y_min;
	std::vector<B> decis;

	Decoder_LDPC_bit_flipping(const int &K, const int &N, const int& n_ite,
	                          const tools::Sparse_matrix &H,
	                          const std::vector<unsigned> &info_bits_pos,
	                          const R mwbf_factor = 0.0f,
	                          const bool enable_syndrome = true,
	                          const int syndrome_depth = 1);
	virtual ~Decoder_LDPC_bit_flipping() = default;
	virtual Decoder_LDPC_bit_flipping<B,R>* clone() const;

	void _reset(const size_t frame_id);

	int _decode_siso   (const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id);
	int _decode_siho   (const R *Y_N,  int8_t *CWD, B *V_K,  const size_t frame_id);
	int _decode_siho_cw(const R *Y_N,  int8_t *CWD, B *V_N,  const size_t frame_id);

	// BP functions for decoding
	bool BF_decode(const R *Y_N, const size_t frame_id);

	virtual bool BF_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V) = 0;

	virtual void set_n_frames(const size_t n_frames);
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_HPP_ */
