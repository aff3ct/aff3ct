#include "Simulation/EXIT/Simulation_EXIT.hpp"

#include "Simulation_EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

template <typename B, typename R>
simulation::Simulation_EXIT<B,R>* Simulation_EXIT
::build(const parameters &params, tools::Codec_SISO<B,R> &codec)
{
	return new simulation::Simulation_EXIT<B,R>(params, codec);
}

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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::Simulation_EXIT<B_8 ,R_8 >* aff3ct::factory::Simulation_EXIT::build<B_8 ,R_8 >(const aff3ct::factory::Simulation_EXIT::parameters&, aff3ct::tools::Codec_SISO<B_8 ,R_8 >&);
template aff3ct::simulation::Simulation_EXIT<B_16,R_16>* aff3ct::factory::Simulation_EXIT::build<B_16,R_16>(const aff3ct::factory::Simulation_EXIT::parameters&, aff3ct::tools::Codec_SISO<B_16,R_16>&);
template aff3ct::simulation::Simulation_EXIT<B_32,R_32>* aff3ct::factory::Simulation_EXIT::build<B_32,R_32>(const aff3ct::factory::Simulation_EXIT::parameters&, aff3ct::tools::Codec_SISO<B_32,R_32>&);
template aff3ct::simulation::Simulation_EXIT<B_64,R_64>* aff3ct::factory::Simulation_EXIT::build<B_64,R_64>(const aff3ct::factory::Simulation_EXIT::parameters&, aff3ct::tools::Codec_SISO<B_64,R_64>&);
#else
template aff3ct::simulation::Simulation_EXIT<B,R>* aff3ct::factory::Simulation_EXIT::build<B,R>(const aff3ct::factory::Simulation_EXIT::parameters&, aff3ct::tools::Codec_SISO<B,R>&);
#endif
// ==================================================================================== explicit template instantiation
