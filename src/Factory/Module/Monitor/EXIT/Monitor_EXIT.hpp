#ifndef FACTORY_MONITOR_EXIT_HPP
#define FACTORY_MONITOR_EXIT_HPP

#include <string>

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"

#include "../Monitor.hpp"

namespace aff3ct
{
namespace factory
{
struct Monitor_EXIT : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Monitor::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename R = float>
		module::Monitor_EXIT<B,R>* build() const;

		int         size     = 0;

		std::string type     = "STD";
		int         n_trials = 200;
		int         n_frames = 1;
	};

	template <typename B = int, typename R = float>
	static module::Monitor_EXIT<B,R>* build(const parameters& params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_mon, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_MONITOR_EXIT_HPP */
