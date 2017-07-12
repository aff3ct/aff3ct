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
struct Factory_encoder_LDPC : public Factory_encoder_common
{
	template <typename B = int>
	static module::Encoder_LDPC<B>* build(const typename Factory_encoder_common::parameters &params,
	                                      const Sparse_matrix &G, const Sparse_matrix &H);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, Factory_encoder_common::parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const Factory_encoder_common::parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_LDPC_HPP */
