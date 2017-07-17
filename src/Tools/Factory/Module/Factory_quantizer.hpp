#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Module/Quantizer/Quantizer.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_quantizer : public Factory
{
	struct parameters
	{
		int         size       = -1;

		std::string type       = "STD";
		float       range      = 0.f;
		float       sigma      = -1.f;
		int         n_bits     = 8;
		int         n_decimals = 3;
		int         n_frames   = 1;
	};

	template <typename R = float, typename Q = R>
	static module::Quantizer<R,Q>* build(const parameters& params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_qua, const parameters& params);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
