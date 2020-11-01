/*!
 * \file
 * \brief Class module::Encoder_LDPC_from_H.
 */
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
	Encoder_LDPC_from_H(const int K, const int N, const tools::Sparse_matrix &H,
		                const std::string& G_method = "IDENTITY", const std::string& G_save_path = "",
		                const bool G_save_path_single_thread = true);
	virtual ~Encoder_LDPC_from_H() = default;

	virtual Encoder_LDPC_from_H<B>* clone() const;
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
