#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"
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
#ifdef MIPP_NO_INTRINSICS
		std::string type       = "STD";
#else
		std::string type       = std::is_same<R,double>::value ? "STD" : "STD_FAST";
#endif
		float       range      = 0.f;
		int         n_bits     = 8;
		int         n_decimals = 3;
	};

	static module::Quantizer<R,Q>* build(const std::string type,
	                                     const int         size,
	                                     const int         n_decimals,
	                                     const int         n_bits,
	                                     const float       sigma    = 0.f,
	                                     const float       range    = 0.f,
	                                     const int         n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, quantizer_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
