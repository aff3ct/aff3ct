#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <vector>

#include "../Encoder_LDPC.hpp"

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_H : public Encoder_LDPC<B>
{
protected:
	tools::Sparse_matrix G; // position of ones by column
	tools::Sparse_matrix H;

public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames = 1);
	virtual ~Encoder_LDPC_from_H();

	bool is_codeword(const B *X_N);

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
