#ifndef ENCODER_LDPC_FROM_QC_HPP_
#define ENCODER_LDPC_FROM_QC_HPP_

#include <vector>

#include "../Encoder_LDPC.hpp"

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_QC : public Encoder_LDPC<B>
{
protected:
	tools::Sparse_matrix H;
	tools::LDPC_matrix_handler::QCFull_matrix invH2;

public:
	Encoder_LDPC_from_QC(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames = 1);
	virtual ~Encoder_LDPC_from_QC();

	bool is_codeword(const B *X_N);

	const std::vector<uint32_t>& get_info_bits_pos();

	bool is_sys() const;

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};

}
}

#endif /* ENCODER_LDPC_FROM_QC_HPP_ */
