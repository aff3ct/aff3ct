#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include <string>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_LDPC : public Factory
{
	static module::Encoder_LDPC<B>* build(const std::string    type,
	                                      const int            K,
	                                      const int            N,
	                                      const Sparse_matrix &G,
	                                      const Sparse_matrix &H,
	                                      const int            n_frames = 1);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
