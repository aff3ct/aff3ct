#ifndef DECODER_LDPC_BIT_FLIPPING_HPP_
#define DECODER_LDPC_BIT_FLIPPING_HPP_

#include <vector>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Decoder/Decoder_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_bit_flipping : public Decoder_SISO_SIHO<B,R>
{
public:
	void reset();

protected:
	const int  n_ite;      // number of iterations to perform
	const int  n_V_nodes;  // number of variable nodes (= N)
	const int  n_C_nodes;  // number of check    nodes (= N - K)
	const int  n_branches; // number of branched in the bi-partite graph (connexions between the V and C nodes)
	const R mwbf_factor;
	const bool enable_syndrome;
	const int  syndrome_depth;

	const tools::Sparse_matrix &H;

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	bool init_flag;

	const std::vector<unsigned> &info_bits_pos;


	std::vector<unsigned char> n_variables_per_parity;
	std::vector<unsigned char> n_parities_per_variable;
	std::vector<unsigned int > transpose;

	// data structures for iterative decoding
	            std::vector<R>  Lp_N;   // a posteriori information
	std::vector<std::vector<R>> C_to_V; // check    nodes to variable nodes messages
	std::vector<std::vector<R>> V_to_C; // variable nodes to check    nodes messages

	Decoder_LDPC_bit_flipping(const int &K, const int &N, const int& n_ite,
	                         const tools::Sparse_matrix &H,
	                         const std::vector<unsigned> &info_bits_pos,
	                         const R mwbf_factor = 0.0f,
	                         const bool enable_syndrome = true,
	                         const int syndrome_depth = 1,
	                         const int n_frames = 1);
	virtual ~Decoder_LDPC_bit_flipping() = default;

	void _decode_siso   (const R *Y_N1, R *Y_N2, const int frame_id);
	void _decode_siho   (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N,  const int frame_id);

	// BP functions for decoding
	void BF_decode(const R *Y_N, const int frame_id);

	virtual bool BF_process(const R *Y_N, std::vector<R> &V_to_C, std::vector<R> &C_to_V) = 0;


	std::vector<R> Y_min;
	std::vector<B> decis;
};
}
}

#endif /* DECODER_LDPC_BIT_FLIPPING_HPP_ */
