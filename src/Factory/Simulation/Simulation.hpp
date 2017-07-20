#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#ifdef ENABLE_MPI
#include <mpi.h>
#endif
#include <chrono>
#include <string>
#include <sstream>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Display/bash_tools.h"

#include "Simulation_main.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation : Simulation_main
{
	struct parameters : Simulation_main::parameters
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
		int                       seed          = 0;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
