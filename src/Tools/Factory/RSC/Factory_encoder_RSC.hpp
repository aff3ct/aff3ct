#ifndef FACTORY_ENCODER_RSC_HPP
#define FACTORY_ENCODER_RSC_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Module/Encoder/RSC/Encoder_RSC_sys.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_RSC : public Factory_encoder_common<B>
{
	struct encoder_parameters_RSC : Factory_encoder_common<B>::encoder_parameters
	{
		virtual ~encoder_parameters_RSC() {}

		bool buffered = true;
	};

	static module::Encoder_RSC_sys<B>* build(const std::string       type,
	                                         const int               K,
	                                         const int               N,
	                                         const bool              buffered = true,
	                                         const std::vector<int>  poly     = {5,7},
	                                               std::ostream     &stream   = std::cout,
	                                         const int               n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, encoder_parameters_RSC &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, const encoder_parameters_RSC& params);
};
}
}

#endif /* FACTORY_ENCODER_RSC_HPP */
