#ifndef ENCODER_LDPC_FROM_SEIRA_HPP_
#define ENCODER_LDPC_FROM_SEIRA_HPP_

#include <vector>

#include "../Encoder_LDPC.hpp"

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_SeIRA : public Encoder_LDPC<B>
{
public:
	Encoder_LDPC_from_SeIRA(const int K, const int N, const tools::Sparse_matrix &H, const int n_frames = 1);
	virtual ~Encoder_LDPC_from_SeIRA() = default;

	const std::vector<uint32_t>& get_info_bits_pos() const;

	bool is_sys() const;

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);
	void _check_H_dimensions();
};

}
}

#endif /* ENCODER_LDPC_FROM_SEIRA_HPP_ */
