#ifndef FACTORY_ENCODER_REPETITION_HPP
#define FACTORY_ENCODER_REPETITION_HPP

#include <string>

#include "Module/Encoder/Encoder_sys.hpp"

#include "../Factory_encoder.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_encoder_repetition : public Factory_encoder
{
	struct parameters : Factory_encoder::parameters
	{
		virtual ~parameters() {}

		bool buffered = true;
	};

	template <typename B = int>
	static module::Encoder_sys<B>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_enc, const parameters& params);
};
}
}

#endif /* FACTORY_ENCODER_REPETITION_HPP */
