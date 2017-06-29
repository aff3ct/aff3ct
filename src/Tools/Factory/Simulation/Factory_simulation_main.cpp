#include <sstream>
#include "Factory_simulation_main.hpp"
#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/types.h"
#include "Tools/date.h"

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

void Factory_simulation_main::header(Header::params_list& head_sim, Header::params_list& head_cde,
                                     const simu_parameters_main& params)
{
	std::unordered_map<std::type_index,std::string> type_names;
	// define type names
	type_names[typeid(char       )] = "char ("        + std::to_string(sizeof(char)*8)        + " bits)";
	type_names[typeid(signed char)] = "signed char (" + std::to_string(sizeof(signed char)*8) + " bits)";
	type_names[typeid(short      )] = "short ("       + std::to_string(sizeof(short)*8)       + " bits)";
	type_names[typeid(int        )] = "int ("         + std::to_string(sizeof(int)*8)         + " bits)";
	type_names[typeid(long long  )] = "long long ("   + std::to_string(sizeof(long long)*8)   + " bits)";
	type_names[typeid(float      )] = "float ("       + std::to_string(sizeof(float)*8)       + " bits)";
	type_names[typeid(double     )] = "double ("      + std::to_string(sizeof(double)*8)      + " bits)";

	// ---------------------------------------------------------------------------------------------------- simulation
	head_sim.push_back(std::make_pair("Type", params.sim_type));

	std::type_index id_B = typeid(char), id_R = typeid(char), id_Q = typeid(char);
#ifdef MULTI_PREC
	switch (params.sim_prec)
	{
		case 8:
			id_B = typeid(B_8);
			id_R = typeid(R_8);
			id_Q = typeid(Q_8);
		break;

		case 16:
			id_B = typeid(B_16);
			id_R = typeid(R_16);
			id_Q = typeid(Q_16);
		break;

		case 32:
			id_B = typeid(B_32);
			id_R = typeid(R_32);
			id_Q = typeid(Q_32);
		break;

#if defined(__x86_64) || defined(__x86_64__) || defined(_WIN64) || defined(__aarch64__)
		case 64:
			id_B = typeid(B_64);
			id_R = typeid(R_64);
			id_Q = typeid(Q_64);
		break;
#endif
		default :
			std::stringstream message;
			message << "Unsupported bit precision: " << params.sim_prec << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		break;
	}
#else
	id_B = typeid(B);
	id_R = typeid(R);
	id_Q = typeid(Q);
#endif

	head_sim.push_back(std::make_pair("Type of bits",  type_names[id_B]));
	head_sim.push_back(std::make_pair("Type of reals", type_names[id_R]));
	if (std::is_integral<Q>::value)
		head_sim.push_back(std::make_pair("Type of quant. reals", type_names[id_Q]));

	using namespace date;
	std::stringstream date;
	date << std::chrono::system_clock::now();
	auto split_date = string_split(date.str(), '.');
	head_sim.push_back(std::make_pair("Date (UTC)", split_date[0]));

	// ---------------------------------------------------------------------------------------------------------- code
	head_cde.push_back(std::make_pair("Type", params.cde_type));
}
