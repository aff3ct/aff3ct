#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>

#include "Module/Monitor/Monitor.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_monitor : public Factory
{
	struct parameters
	{
		int         size             = 0;

		std::string type             = "STD";
		std::string err_track_path   = "error_tracker";
		bool        err_track_revert = false;
		bool        err_track_enable = false;
		int         n_frame_errors   = 100;
		int         n_frames         = 1;
	};

	template <typename B = int>
	static module::Monitor<B>* build(const parameters& params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_mon, const parameters& params);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
