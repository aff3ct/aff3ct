#ifndef FACTORY_TERMINAL_HPP_
#define FACTORY_TERMINAL_HPP_

#include <string>
#include <chrono>

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Terminal : Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}

		std::chrono::milliseconds frequency = std::chrono::milliseconds(500);
		bool                      disabled  = false;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_ter, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_TERMINAL_HPP_ */
