#include "Simulation/BFER/Standard/StarPU/SPU_BFER_std.hpp"
#include "Simulation/BFER/Standard/SystemC/SC_BFER_std.hpp"
#include "Simulation/BFER/Standard/Threads/BFER_std_threads.hpp"

#include "BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::BFER_std::name   = "Simulation BFER standard";
const std::string aff3ct::factory::BFER_std::prefix = "sim";

template <typename B, typename R, typename Q>
simulation::BFER_std<B,R,Q>* BFER_std
::build(const parameters &params, tools::Codec<B,Q> &codec)
{
#if defined(SYSTEMC)
	return new simulation::SC_Simulation_BFER_std     <B,R,Q>(params, codec);
#elif defined(STARPU)
	return new simulation::SPU_Simulation_BFER_std    <B,R,Q>(params, codec);
#else
	return new simulation::BFER_std_threads<B,R,Q>(params, codec);
#endif
}

void BFER_std::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	BFER::build_args(req_args, opt_args);

#if !defined(STARPU) && !defined(SYSTEMC)
	opt_args[{p+"-debug-fe"}] =
		{"",
		 "enable debug mode: print array values after each step (only when frame errors)."};
#endif
}

void BFER_std::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	// need to be checked before that the default number of threads is set to 1 because debug mode is selected
#if !defined(STARPU) && !defined(SYSTEMC)
	if(exist(vals, {p+"-debug-fe"}))
	{
		params.debug    = true;
		params.debug_fe = true;
	}
#endif

	BFER::store_args(vals, params);
}

void BFER_std::header(params_list& head_sim, const parameters& params)
{
	BFER::header(head_sim, params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template aff3ct::simulation::BFER_std<B_8 ,R_8 ,Q_8 >* aff3ct::factory::BFER_std::build<B_8 ,R_8 ,Q_8 >(const aff3ct::factory::BFER_std::parameters&, aff3ct::tools::Codec<B_8 ,Q_8 >&);
template aff3ct::simulation::BFER_std<B_16,R_16,Q_16>* aff3ct::factory::BFER_std::build<B_16,R_16,Q_16>(const aff3ct::factory::BFER_std::parameters&, aff3ct::tools::Codec<B_16,Q_16>&);
template aff3ct::simulation::BFER_std<B_32,R_32,Q_32>* aff3ct::factory::BFER_std::build<B_32,R_32,Q_32>(const aff3ct::factory::BFER_std::parameters&, aff3ct::tools::Codec<B_32,Q_32>&);
template aff3ct::simulation::BFER_std<B_64,R_64,Q_64>* aff3ct::factory::BFER_std::build<B_64,R_64,Q_64>(const aff3ct::factory::BFER_std::parameters&, aff3ct::tools::Codec<B_64,Q_64>&);
#else
template aff3ct::simulation::BFER_std<B,R,Q>* aff3ct::factory::BFER_std::build<B,R,Q>(const aff3ct::factory::BFER_std::parameters&, aff3ct::tools::Codec<B,Q>&);
#endif
// ==================================================================================== explicit template instantiation
