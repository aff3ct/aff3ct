#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include "Module/Monitor/Monitor.hpp"

#include <string>

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Monitor : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_mnt, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
