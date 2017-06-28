#include "Factory_simulation_main.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_main::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ---------------------------------------------------------------------------------------------------- simulation
	opt_args[{"sim-type"}] =
		{"string",
		 "select the type of simulation to launch (default is BFER).",
		 "BFER, BFERI, EXIT, GEN"};

#ifdef MULTI_PREC
	opt_args[{"sim-prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	req_args[{"cde-type"}] =
		{"string",
		 "select the code type you want to use.",
		 "POLAR, TURBO, LDPC, REPETITION, RA, RSC, BCH, UNCODED"};

	// -------------------------------------------------------------------------------------------------------- others
	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};

	opt_args[{"version", "v"}] =
		{"",
		 "print informations about the version of the code."};
}

void Factory_simulation_main::store_args(const Arguments_reader& ar, simu_parameters_main &params)
{
	// ---------------------------------------------------------------------------------------------------- simulation
	if(ar.exist_arg({"sim-type"})) params.sim_type = ar.get_arg({"sim-type"});

#ifdef MULTI_PREC
	if(ar.exist_arg({"sim-prec", "p"})) params.sim_prec = ar.get_arg_int({"sim-prec", "p"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	params.cde_type = ar.get_arg({"cde-type"}); // required

	// -------------------------------------------------------------------------------------------------------- others
	if(ar.exist_arg({"help",    "h"})) params.display_help    = true;
	if(ar.exist_arg({"version", "v"})) params.display_version = true;
}

void Factory_simulation_main::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"sim", "Simulation parameter(s)"});
	ar.push_back({"cde", "Code parameter(s)"      });
}
