#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <vector>
#include <thread>

#include "../Encoder_LDPC.hpp"

#include "Tools/Code/LDPC/Matrix_handler/LDPC_matrix_handler.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_from_H : public Encoder_LDPC<B>
{
private:
	static std::thread::id master_thread_id;

public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H, const std::string& G_method = "FAST",
	                    const std::string& G_save_path = "", const bool G_save_path_single_thread = true,
	                    const int n_frames = 1);
	virtual ~Encoder_LDPC_from_H() = default;
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
