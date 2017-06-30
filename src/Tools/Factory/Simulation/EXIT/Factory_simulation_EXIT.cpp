#include "Factory_simulation_EXIT.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_EXIT::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------- simulation
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

void Factory_simulation_EXIT::store_args(const Arguments_reader& ar, simu_parameters_EXIT &params)
{
	Factory_simulation::store_args(ar, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	if(ar.exist_arg({"sim-siga-min", "a"})) params.sig_a_min  = ar.get_arg_float({"sim-siga-min", "a"});
	if(ar.exist_arg({"sim-siga-max", "A"})) params.sig_a_max  = ar.get_arg_float({"sim-siga-max", "A"});
	if(ar.exist_arg({"sim-siga-step"    })) params.sig_a_step = ar.get_arg_float({"sim-siga-step"    });

}

void Factory_simulation_EXIT::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation::group_args(ar);
}

void Factory_simulation_EXIT::header(Header::params_list& head_sim, Header::params_list& head_cde,
                                     const simu_parameters_EXIT& params)
{
	Factory_simulation::header(head_sim, head_cde, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	head_sim.push_back(std::make_pair("Sigma-a min (a)", std::to_string(params.sig_a_min )));
	head_sim.push_back(std::make_pair("Sigma-a max (A)", std::to_string(params.sig_a_max )));
	head_sim.push_back(std::make_pair("Sigma-a step",    std::to_string(params.sig_a_step)));
}
