#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <string>
#include <thread>

#include "Tools/Algo/Matrix/Sparse_matrix/Sparse_matrix.hpp"
#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

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
