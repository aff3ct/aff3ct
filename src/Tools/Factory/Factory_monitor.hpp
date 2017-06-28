#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include <string>

#include "Module/Monitor/Monitor.hpp"

#include "Tools/Arguments_reader.hpp"
#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_monitor : public Factory
{
	struct monitor_parameters
	{
		int         n_frame_errors   = 100;
		bool        err_track_revert = false;
		bool        err_track_enable = false;
		std::string err_track_path   = "error_tracker";
	};

	static module::Monitor<B>* build(const std::string type,
	                                 const int         K,
	                                 const int         fe       = 100,
	                                 const int         n_frames = 0);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, monitor_parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
