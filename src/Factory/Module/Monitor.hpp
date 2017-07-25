#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>

#include "Module/Monitor/Monitor.hpp"

#include "../Factory.hpp"

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
		int         size           = 0;

		std::string type           = "STD";
		int         n_frame_errors = 100;
		int         n_frames       = 1;
	};

	template <typename B = int>
	static module::Monitor<B>* build(const parameters& params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_mon, const parameters& params);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
