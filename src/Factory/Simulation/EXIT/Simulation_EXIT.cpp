#include "Simulation_EXIT.hpp"

using namespace aff3ct::factory;

void Simulation_EXIT::build_args(arg_map &req_args, arg_map &opt_args)
{
	Simulation::build_args(req_args, opt_args);

	req_args[{"sim-siga-min", "a"}] =
		{"positive_float",
		 "sigma min value used in EXIT charts."};

	req_args[{"sim-siga-max", "A"}] =
		{"positive_float",
		 "sigma max value used in EXIT charts."};

	opt_args[{"sim-siga-step"}] =
		{"positive_float",
		 "sigma step value used in EXIT charts."};

	opt_args[{"sim-inter-lvl"}].push_back("1");
}

void Simulation_EXIT::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	Simulation::store_args(ar, params);

	if(ar.exist_arg({"sim-siga-min", "a"})) params.sig_a_min  = ar.get_arg_float({"sim-siga-min", "a"});
	if(ar.exist_arg({"sim-siga-max", "A"})) params.sig_a_max  = ar.get_arg_float({"sim-siga-max", "A"});
	if(ar.exist_arg({"sim-siga-step"    })) params.sig_a_step = ar.get_arg_float({"sim-siga-step"    });
}

void Simulation_EXIT::group_args(arg_grp& ar)
{
	Simulation::group_args(ar);
}

void Simulation_EXIT::header(params_list& head_sim, const parameters& params)
{
	Simulation::header(head_sim, params);

	head_sim.push_back(std::make_pair("Sigma-a min (a)", std::to_string(params.sig_a_min )));
	head_sim.push_back(std::make_pair("Sigma-a max (A)", std::to_string(params.sig_a_max )));
	head_sim.push_back(std::make_pair("Sigma-a step",    std::to_string(params.sig_a_step)));
}
