#include "Factory_simulation_GEN.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_GEN::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation_main::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------- simulation

}

void Factory_simulation_GEN::store_args(const Arguments_reader& ar, parameters &params)
{
	Factory_simulation_main::store_args(ar, params);

	// ---------------------------------------------------------------------------------------------------- simulation

	// ---------------------------------------------------------------------------------------------------------- code

}

void Factory_simulation_GEN::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation_main::group_args(ar);
}

void Factory_simulation_GEN::header(Header::params_list& head_sim, Header::params_list& head_cde,
                                     const parameters& params)
{
	Factory_simulation_main::header(head_sim, head_cde, params);

	// ---------------------------------------------------------------------------------------------------- simulation


	// ---------------------------------------------------------------------------------------------------------- code


}
