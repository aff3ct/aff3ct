#ifndef FACTORY_ENCODER_POLAR_HPP
#define FACTORY_ENCODER_POLAR_HPP

#include <string>
#include <mipp.h>

#include "Module/Encoder/Encoder.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_polar : public Factory_encoder_common<B>
{
	static module::Encoder<B>* build(const typename Factory_encoder_common<B>::encoder_parameters& params,
	                                 const mipp::vector<B> &frozen_bits);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, typename Factory_encoder_common<B>::encoder_parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const typename Factory_encoder_common<B>::encoder_parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_POLAR_HPP */
