#include <thread>

#include "BFER.hpp"

using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER::name   = "Simulation BFER";
const std::string aff3ct::factory::BFER::prefix = "sim";

void BFER::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Simulation::build_args(req_args, opt_args);

	opt_args[{p+"-benchs", "b"}] =
		{"positive_int",
		 "enable special benchmark mode with a loop around the decoder."};

	opt_args[{p+"-debug", "d"}] =
		{"",
		 "enable debug mode: print array values after each step."};

	opt_args[{p+"-debug-prec"}] =
		{"positive_int",
		 "set the precision of real elements when displayed in debug mode."};

	opt_args[{p+"-debug-limit"}] =
		{"positive_int",
		 "set the max number of elements to display in the debug mode."};

	opt_args[{p+"-time-report"}] =
		{"",
		 "display time information about the simulation chain."};

	opt_args[{p+"-snr-type", "E"}] =
		{"string",
		 "select the type of SNR: symbol energy or information bit energy.",
		 "ES, EB"};

	opt_args[{p+"-coset", "c"}] =
		{"",
		 "enable the coset approach."};
}

void BFER::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
#if !defined(STARPU) && !defined(SYSTEMC)
	params.n_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1;
#endif

	Simulation::store_args(vals, params);

	if(exist(vals, {p+"-benchs",     "b"})) params.benchs      = std::stoi(vals.at({p+"-benchs",   "b"}));
	if(exist(vals, {p+"-snr-type",   "E"})) params.snr_type    =           vals.at({p+"-snr-type", "E"});
	if(exist(vals, {p+"-time-report"    })) params.time_report = true;
	if(exist(vals, {p+"-debug",      "d"})) params.debug       = true;
	if(exist(vals, {p+"-coset",      "c"})) params.coset       = true;
	if(exist(vals, {p+"-debug-limit"}))
	{
		params.debug = true;
		params.debug_limit = std::stoi(vals.at({p+"-debug-limit"}));
	}
	if(exist(vals, {p+"-debug-prec"}))
	{
		params.debug = true;
		params.debug_precision = std::stoi(vals.at({p+"-debug-prec"}));
	}

	if (params.debug && !(exist(vals, {p+"-threads", "t"}) && std::stoi(vals.at({p+"-threads", "t"})) > 0))
		// check if debug is asked and if n_thread kept its default value
		params.n_threads = 1;

}

void BFER::header(params_list& head_sim, const parameters& params)
{
	Simulation::header(head_sim, params);

	if (params.benchs)
		head_sim.push_back(std::make_pair("Number of benchs", std::to_string(params.benchs)));
	head_sim.push_back(std::make_pair("SNR type", params.snr_type));
	head_sim.push_back(std::make_pair("Time report", params.time_report ? "on" : "off"));
	head_sim.push_back(std::make_pair("Debug mode", params.debug ? "on" : "off"));
	if (params.debug)
	{
		head_sim.push_back(std::make_pair("Debug precision", std::to_string(params.debug_precision)));
		if (params.debug_limit)
			head_sim.push_back(std::make_pair("Debug limit", std::to_string(params.debug_limit)));
	}
	head_sim.push_back(std::make_pair("Coset approach (c)", params.coset ? "yes" : "no"));
}
