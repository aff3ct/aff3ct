#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>

#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory_encoder_common.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_encoder_repetition : public Factory_encoder_common<B>
{
	struct encoder_parameters_repetition : Factory_encoder_common<B>::encoder_parameters
	{
		virtual ~encoder_parameters_repetition() {}

		bool buffered = true;
	};

	static module::Encoder_sys<B>* build(const encoder_parameters_repetition &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, encoder_parameters_repetition &params,
	                       const int K, const int N, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_enc, Header::params_list& head_cde, const encoder_parameters_repetition& params);
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
