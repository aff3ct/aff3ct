#include "Simulation/BFER/Iterative/SystemC/SC_BFER_ite.hpp"
#include "Simulation/BFER/Iterative/Threads/BFER_ite_threads.hpp"

#include "BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_ite::name   = "Simulation BFER iterative";
const std::string aff3ct::factory::BFER_ite::prefix = "sim";

template <typename B, typename R, typename Q>
simulation::BFER_ite<B,R,Q>* BFER_ite
::build(const parameters &params, tools::Codec_SISO<B,Q> &codec)
{
#if defined(SYSTEMC)
	return new simulation::SC_Simulation_BFER_ite     <B,R,Q>(params, codec);
#else
	return new simulation::BFER_ite_threads<B,R,Q>(params, codec);
#endif
}

void BFER_ite::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	BFER::build_args(req_args, opt_args, p);

	opt_args[{p+"-ite", "I"}] =
		{"positive_int",
		 "number of global iterations between the demodulator and the decoder."};
}

void BFER_ite::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	BFER::store_args(vals, params, p);

	if(exist(vals, {p+"-ite", "I"})) params.n_ite = std::stoi(vals.at({p+"-ite", "I"}));
}

void BFER_ite::make_header(params_list& head_sim, const parameters& params)
{
	BFER::make_header(head_sim, params);

	head_sim.push_back(std::make_pair("Global iterations (I)", std::to_string(params.n_ite)));
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::BFER_ite<B_8 ,R_8 ,Q_8 >* aff3ct::factory::BFER_ite::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::BFER_ite::parameters&, aff3ct::tools::Codec_SISO<B_8 ,Q_8 >&);
template aff3ct::simulation::BFER_ite<B_16,R_16,Q_16>* aff3ct::factory::BFER_ite::build<B_16,R_16,Q_16>(const aff3ct::factory::BFER_ite::parameters&, aff3ct::tools::Codec_SISO<B_16,Q_16>&);
template aff3ct::simulation::BFER_ite<B_32,R_32,Q_32>* aff3ct::factory::BFER_ite::build<B_32,R_32,Q_32>(const aff3ct::factory::BFER_ite::parameters&, aff3ct::tools::Codec_SISO<B_32,Q_32>&);
template aff3ct::simulation::BFER_ite<B_64,R_64,Q_64>* aff3ct::factory::BFER_ite::build<B_64,R_64,Q_64>(const aff3ct::factory::BFER_ite::parameters&, aff3ct::tools::Codec_SISO<B_64,Q_64>&);
#else
template aff3ct::simulation::BFER_ite<B,R,Q>* aff3ct::factory::BFER_ite::build<B,R,Q>(const aff3ct::factory::BFER_ite::parameters&, aff3ct::tools::Codec_SISO<B,Q>&);
#endif
// ==================================================================================== explicit template instantiation
