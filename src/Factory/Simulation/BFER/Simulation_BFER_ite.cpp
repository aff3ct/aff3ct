#include "Simulation_BFER_ite.hpp"

using namespace aff3ct::factory;

void Simulation_BFER_ite::build_args(arg_map &req_args, arg_map &opt_args)
{
	Simulation_BFER::build_args(req_args, opt_args);

	opt_args[{"sim-ite", "I"}] =
		{"positive_int",
		 "number of global iterations between the demodulator and the decoder."};

}

void Simulation_BFER_ite::store_args(const tools::Arguments_reader& ar, parameters &params)
{
	Simulation_BFER::store_args(ar, params);

	if(ar.exist_arg({"sim-ite", "I"})) params.n_ite = ar.get_arg_int({"sim-ite", "I"});
}

void Simulation_BFER_ite::group_args(arg_grp& ar)
{
	Simulation_BFER::group_args(ar);
}

void Simulation_BFER_ite::header(params_list& head_sim, const parameters& params)
{
	Simulation_BFER::header(head_sim, params);

	head_sim.push_back(std::make_pair("Global iterations (I)", std::to_string(params.n_ite)));
}
