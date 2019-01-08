#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <vector>

#include "../Encoder_LDPC.hpp"

#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_H : public Encoder_LDPC<B>
{
public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const std::string& G_method = "FAST",
	                    const std::string& G_save_path = "", const int n_frames = 1);
	virtual ~Encoder_LDPC_from_H() = default;
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
