#ifndef FACTORY_QUANTIZER_HPP
#define FACTORY_QUANTIZER_HPP

#include <string>

#include "Module/Quantizer/Quantizer.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Quantizer : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		template <typename R = float, typename Q = R>
		module::Quantizer<R,Q>* build() const;

		int         size       = 0;

		std::string type       = "STD";
		float       range      = 0.f;
		int         n_bits     = 8;
		int         n_decimals = 3;
		int         n_frames   = 1;
	};

	template <typename R = float, typename Q = R>
	static module::Quantizer<R,Q>* build(const parameters& params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_qua, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_QUANTIZER_HPP */
