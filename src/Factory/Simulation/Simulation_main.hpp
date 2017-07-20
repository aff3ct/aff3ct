#ifndef FACTORY_SIMULATION_MAIN_H_
#define FACTORY_SIMULATION_MAIN_H_

#include <string>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation_main : public Factory
{
	struct parameters
	{
		std::string cde_type;

		std::string sim_type        = "BFER";
		int         sim_prec        = 32;
		bool        display_help    = false;
		bool        display_version = false;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters& params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_MAIN_H_ */
