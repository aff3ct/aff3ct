#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Factory_simulation_BFER.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER_std : Factory_simulation_BFER
{
	struct parameters : Factory_simulation_BFER::parameters
	{
		parameters() : Factory_simulation_BFER::parameters() {}

		virtual ~parameters() {}

		bool debug_fe = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
