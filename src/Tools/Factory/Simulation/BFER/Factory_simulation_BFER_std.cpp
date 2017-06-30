#include "Factory_simulation_BFER_std.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_BFER_std::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation_BFER::build_args(req_args, opt_args);

#if !defined(STARPU) && !defined(SYSTEMC)
	opt_args[{"sim-debug-fe"}] =
		{"",
		 "enable debug mode: print array values after each step (only when frame errors)."};
#endif
}

void Factory_simulation_BFER_std::store_args(const Arguments_reader& ar, simu_parameters_BFER_std &params)
{
	// need to be checked before that the default number of threads is set to 1 because debug mode is selected
#if !defined(STARPU) && !defined(SYSTEMC)
	if(ar.exist_arg({"sim-debug-fe"}))
	{
		params.debug    = true;
		params.debug_fe = true;
	}
#endif

	Factory_simulation_BFER::store_args(ar, params);
}

void Factory_simulation_BFER_std::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation_BFER::group_args(ar);
}

void Factory_simulation_BFER_std::header(Header::params_list& head_sim, Header::params_list& head_cde,
                                         const simu_parameters_BFER_std& params)
{
	Factory_simulation_BFER::header(head_sim, head_cde, params);
}
