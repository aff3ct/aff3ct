#include "Simulation/EXIT/EXIT.hpp"

#include "EXIT.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::EXIT::name   = "Simulation EXIT";
const std::string aff3ct::factory::EXIT::prefix = "sim";

template <typename B, typename R>
simulation::EXIT<B,R>* EXIT
::build(const parameters &params, tools::Codec_SISO<B,R> &codec)
{
	return new simulation::EXIT<B,R>(params, codec);
}

void EXIT::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Simulation::build_args(req_args, opt_args);

	req_args[{p+"-siga-min", "a"}] =
		{"positive_float",
		 "sigma min value used in EXIT charts."};

	req_args[{p+"-siga-max", "A"}] =
		{"positive_float",
		 "sigma max value used in EXIT charts."};

	opt_args[{p+"-siga-step"}] =
		{"positive_float",
		 "sigma step value used in EXIT charts."};

	opt_args[{p+"-inter-lvl"}].push_back("1");
}

void EXIT::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	Simulation::store_args(vals, params);

	if(exist(vals, {p+"-siga-min", "a"})) params.sig_a_min  = std::stof(vals.at({p+"-siga-min", "a"}));
	if(exist(vals, {p+"-siga-max", "A"})) params.sig_a_max  = std::stof(vals.at({p+"-siga-max", "A"}));
	if(exist(vals, {p+"-siga-step"    })) params.sig_a_step = std::stof(vals.at({p+"-siga-step"    }));
}

void EXIT::make_header(params_list& head_sim, const parameters& params)
{
	Simulation::make_header(head_sim, params);

	head_sim.push_back(std::make_pair("Sigma-a min (a)", std::to_string(params.sig_a_min )));
	head_sim.push_back(std::make_pair("Sigma-a max (A)", std::to_string(params.sig_a_max )));
	head_sim.push_back(std::make_pair("Sigma-a step",    std::to_string(params.sig_a_step)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::EXIT<B_8 ,R_8 >* aff3ct::factory::EXIT::build<B_8 ,R_8 >(const aff3ct::factory::EXIT::parameters&, aff3ct::tools::Codec_SISO<B_8 ,R_8 >&);
template aff3ct::simulation::EXIT<B_16,R_16>* aff3ct::factory::EXIT::build<B_16,R_16>(const aff3ct::factory::EXIT::parameters&, aff3ct::tools::Codec_SISO<B_16,R_16>&);
template aff3ct::simulation::EXIT<B_32,R_32>* aff3ct::factory::EXIT::build<B_32,R_32>(const aff3ct::factory::EXIT::parameters&, aff3ct::tools::Codec_SISO<B_32,R_32>&);
template aff3ct::simulation::EXIT<B_64,R_64>* aff3ct::factory::EXIT::build<B_64,R_64>(const aff3ct::factory::EXIT::parameters&, aff3ct::tools::Codec_SISO<B_64,R_64>&);
#else
template aff3ct::simulation::EXIT<B,R>* aff3ct::factory::EXIT::build<B,R>(const aff3ct::factory::EXIT::parameters&, aff3ct::tools::Codec_SISO<B,R>&);
#endif
// ==================================================================================== explicit template instantiation
