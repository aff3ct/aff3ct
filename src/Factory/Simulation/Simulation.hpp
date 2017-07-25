#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#ifdef ENABLE_MPI
#include <mpi.h>
#endif
#include <chrono>
#include <string>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Factory/Launcher/Launcher.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation : Launcher
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Launcher::parameters
	{
		virtual ~parameters() {}

		float                     snr_min;
		float                     snr_max;

#ifdef ENABLE_MPI
		std::chrono::milliseconds mpi_comm_freq = std::chrono::milliseconds(1000);
		int                       mpi_rank      = 0;
		int                       mpi_size      = 1;
#endif
		std::chrono::seconds      stop_time     = std::chrono::seconds(0);
		std::string               pyber         = "";
		float                     snr_step      = 0.1f;
		int                       n_threads     = 1;
		int                       local_seed    = 0;
		int                       global_seed   = 0;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
