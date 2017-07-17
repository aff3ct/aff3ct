#include "Factory_simulation_BFER.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_BFER::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------- simulation
	opt_args[{"sim-benchs", "b"}] =
		{"positive_int",
		 "enable special benchmark mode with a loop around the decoder."};

	opt_args[{"sim-debug", "d"}] =
		{"",
		 "enable debug mode: print array values after each step."};

	opt_args[{"sim-debug-prec"}] =
		{"positive_int",
		 "set the precision of real elements when displayed in debug mode."};

	opt_args[{"sim-debug-limit"}] =
		{"positive_int",
		 "set the max number of elements to display in the debug mode."};

	opt_args[{"sim-time-report"}] =
		{"",
		 "display time information about the simulation chain."};

	opt_args[{"sim-snr-type", "E"}] =
		{"string",
		 "select the type of SNR: symbol energy or information bit energy.",
		 "ES, EB"};
}

void Factory_simulation_BFER::store_args(const Arguments_reader& ar, parameters &params)
{
#if !defined(STARPU) && !defined(SYSTEMC)
	params.n_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
#endif

	Factory_simulation::store_args(ar, params);

	if(ar.exist_arg({"sim-benchs",     "b"})) params.benchs      = ar.get_arg_int({"sim-benchs",   "b"});
	if(ar.exist_arg({"sim-snr-type",   "E"})) params.snr_type    = ar.get_arg    ({"sim-snr-type", "E"});
	if(ar.exist_arg({"sim-time-report"    })) params.time_report = true;
	if(ar.exist_arg({"sim-debug",      "d"})) params.debug       = true;
	if(ar.exist_arg({"sim-debug-limit"}))
	{
		params.debug = true;
		params.debug_limit = ar.get_arg_int({"sim-debug-limit"});
	}
	if(ar.exist_arg({"sim-debug-prec"}))
	{
		params.debug = true;
		params.debug_precision = ar.get_arg_int({"sim-debug-prec"});
	}

	if (params.debug && !(ar.exist_arg({"sim-threads", "t"}) && ar.get_arg_int({"sim-threads", "t"}) > 0))
		// check if debug is asked and if n_thread kept its default value
		params.n_threads = 1;

}

void Factory_simulation_BFER::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation::group_args(ar);
}

void Factory_simulation_BFER::header(params_list& head_sim, const parameters& params)
{
	Factory_simulation::header(head_sim, params);
}
