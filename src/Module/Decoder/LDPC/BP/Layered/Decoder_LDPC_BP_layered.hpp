#ifndef DECODER_LDPC_BP_LAYERED_HPP_
#define DECODER_LDPC_BP_LAYERED_HPP_

#include "../../../Decoder_SISO_SIHO.hpp"
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_layered : public Decoder_SISO_SIHO<B,R>
{
protected:
	const int n_ite;     // number of iterations to perform
	const int n_C_nodes; // number of check nodes (= N - K)

	const bool enable_syndrome;
	const int  syndrome_depth;

	// reset so C_to_V and V_to_C structures can be cleared only at the beginning of the loop in iterative decoding
	bool init_flag;

	const std::vector<unsigned> &info_bits_pos;

	const tools::Sparse_matrix &H;

	// data structures for iterative decoding
	std::vector<std::vector<R>> var_nodes;
	std::vector<std::vector<R>> branches;

public:
	Decoder_LDPC_BP_layered(const int &K, const int &N, const int& n_ite,
	                        const tools::Sparse_matrix &H,
	                        const std::vector<unsigned> &info_bits_pos,
	                        const bool enable_syndrome = true,
	                        const int syndrome_depth = 1,
	                        const int n_frames = 1,
	                        const std::string name = "Decoder_LDPC_BP_layered");
	virtual ~Decoder_LDPC_BP_layered();

protected:
	void _load             (const R *Y_N,           const int frame_id);
	void _decode_siso      (const R *Y_N1, R *Y_N2, const int frame_id);
	void __decode_siho     (const R *Y_N,           const int frame_id);
	void _decode_siho      (const R *Y_N,  B *V_K,  const int frame_id);
	void _decode_siho_coded(const R *Y_N,  B *V_N,  const int frame_id);

	// BP functions for decoding
	void BP_decode(const int frame_id);

	bool check_syndrome(const int frame_id);

	virtual void BP_process(std::vector<R> &var_nodes, std::vector<R> &branches) = 0;
};
}
}

#endif /* DECODER_LDPC_BP_LAYERED_HPP_ */
