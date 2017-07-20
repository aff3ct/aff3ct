#include "Simulation_BFER_std.hpp"

using namespace aff3ct::factory;

void Simulation_BFER_std::build_args(arg_map &req_args, arg_map &opt_args)
{
	Simulation_BFER::build_args(req_args, opt_args);

#if !defined(STARPU) && !defined(SYSTEMC)
	opt_args[{"sim-debug-fe"}] =
		{"",
		 "enable debug mode: print array values after each step (only when frame errors)."};
#endif
}

void Simulation_BFER_std::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	// need to be checked before that the default number of threads is set to 1 because debug mode is selected
#if !defined(STARPU) && !defined(SYSTEMC)
	if(ar.exist_arg({"sim-debug-fe"}))
	{
		params.debug    = true;
		params.debug_fe = true;
	}
#endif

	Simulation_BFER::store_args(ar, params);
}

void Simulation_BFER_std::group_args(arg_grp& ar)
{
	Simulation_BFER::group_args(ar);
}

void Simulation_BFER_std::header(params_list& head_sim, const parameters& params)
{
	Simulation_BFER::header(head_sim, params);
}
