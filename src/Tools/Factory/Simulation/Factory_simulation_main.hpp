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
namespace tools
{

struct Factory_simulation_main : public Factory
{
	struct parameters
	{
		std::string cde_type;

		std::string sim_type        = "BFER";
		int         sim_prec        = 32;
		bool        display_help    = false;
		bool        display_version = false;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_SIMULATION_MAIN_H_ */
