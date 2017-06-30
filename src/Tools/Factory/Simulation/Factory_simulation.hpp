#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#ifdef ENABLE_MPI
#include <mpi.h>
#endif
#include <chrono>
#include <string>
#include <sstream>

#include "Tools/Arguments_reader.hpp"
#include "Factory_simulation_main.hpp"
#include "Tools/Display/bash_tools.h"

namespace aff3ct
{
namespace tools
{
struct Factory_simulation : Factory_simulation_main
{
	struct simu_parameters : simu_parameters_main
	{
		// ---- simulation
#ifdef ENABLE_MPI
		std::chrono::milliseconds mpi_comm_freq     = std::chrono::milliseconds(1000);
		int                       mpi_rank          = 0;
		int                       mpi_size          = 1;
#endif
		std::chrono::seconds      stop_time         = std::chrono::seconds(0);
		std::string               pyber             = "";
		float                     snr_min;
		float                     snr_max;
		float                     snr_step          = 0.1f;
		int                       n_threads         = 1;
		int                       inter_frame_level = 1;
		int                       seed              = 0;

		// ---- code
		int         K;
		int         K_info;
		int         N;
		int         N_code;
		float       R;     // code rate
//		int         tail_length = 0;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sim, Header::params_list& head_cde, const simu_parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
