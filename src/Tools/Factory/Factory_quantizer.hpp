#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"
#include "Tools/Header.hpp"
#include "Module/Quantizer/Quantizer.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, typename Q = R>
struct Factory_quantizer : public Factory
{
	struct quantizer_parameters
	{
		std::string type       = "STD";
		float       range      = 0.f;
		int         n_bits     = 8;
		int         n_decimals = 3;

		int         size;
		int         n_frames;
	};

	static module::Quantizer<R,Q>* build(const quantizer_parameters& params, const float sigma = 0.f);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, quantizer_parameters &params,
	                       const int size, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_qua, const quantizer_parameters& params);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
