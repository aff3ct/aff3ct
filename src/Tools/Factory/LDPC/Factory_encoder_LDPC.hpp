#ifndef FACTORY_ENCODER_LDPC_HPP
#define FACTORY_ENCODER_LDPC_HPP

#include <string>
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Module/Encoder/LDPC/Encoder_LDPC.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_LDPC : public Factory_encoder_common<B>
{
	static module::Encoder_LDPC<B>* build(const std::string    type,
	                                      const int            K,
	                                      const int            N,
	                                      const Sparse_matrix &G,
	                                      const Sparse_matrix &H,
	                                      const int            n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, const typename Factory_encoder_common<B>::encoder_parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
