#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Simulation_BFER.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation_BFER_std : Simulation_BFER
{
	struct parameters : Simulation_BFER::parameters
	{
		parameters() : Simulation_BFER::parameters() {}

		virtual ~parameters() {}

		bool debug_fe = false;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
