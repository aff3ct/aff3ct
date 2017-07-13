#include "Factory_simulation_BFER_ite.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation_BFER_ite::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation_BFER::build_args(req_args, opt_args);

	// ---------------------------------------------------------------------------------------------------- simulation
	opt_args[{"sim-ite", "I"}] =
		{"positive_int",
		 "number of global iterations between the demodulator and the decoder."};

}

void Factory_simulation_BFER_ite::store_args(const Arguments_reader& ar, parameters &params)
{
	Factory_simulation_BFER::store_args(ar, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	if(ar.exist_arg({"sim-ite", "I"})) params.n_ite = ar.get_arg_int({"sim-ite", "I"});
}

void Factory_simulation_BFER_ite::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation_BFER::group_args(ar);
}

void Factory_simulation_BFER_ite::header(params_list& head_sim, params_list& head_cde,
                                         const parameters& params)
{
	Factory_simulation_BFER::header(head_sim, head_cde, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	head_sim.push_back(std::make_pair("Global iterations (I)", std::to_string(params.n_ite)));
}
