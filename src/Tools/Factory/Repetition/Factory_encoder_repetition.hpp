#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>

#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_encoder_repetition : public Factory_encoder_common
{
	struct parameters : Factory_encoder_common::parameters
	{
		virtual ~parameters() {}

		bool buffered = true;
	};

	template <typename B = int>
	static module::Encoder_sys<B>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
