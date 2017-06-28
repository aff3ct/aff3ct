#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>
#include <thread>

#include "Factory_simulation.hpp"
#include "Tools/Arguments_reader.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER : Factory_simulation
{
	struct simu_parameters_BFER : simu_parameters
	{
		int         benchs          = 0;
		std::string snr_type        = "EB";
		bool        time_report     = false;
		bool        debug           = false;
		int         debug_limit     = 0;
		int         debug_precision = 5;

		bool        coset           = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters_BFER &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
