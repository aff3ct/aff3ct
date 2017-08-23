#include "Simulation/BFER/Standard/StarPU/SPU_BFER_std.hpp"
#include "Simulation/BFER/Standard/SystemC/SC_BFER_std.hpp"
#include "Simulation/BFER/Standard/Threads/BFER_std_threads.hpp"

#include "BFER_std.hpp"

namespace aff3ct
{
namespace factory
{
template <class C>
template <typename B, typename R, typename Q, int CRC, int ITL>
simulation::BFER_std<C,B,R,Q,CRC,ITL>* BFER_std::parameters<C>
::build() const
{
#if defined(SYSTEMC)
	return new simulation::SC_BFER_std<C,B,R,Q,CRC,ITL>(*this);
#elif defined(STARPU)
	return new simulation::SPU_BFER_std<C,B,R,Q,CRC,ITL>(*this);
#else
	return new simulation::BFER_std_threads<C,B,R,Q,CRC,ITL>(*this);
#endif
}

template <class C, typename B, typename R, typename Q, int CRC, int ITL>
simulation::BFER_std<C,B,R,Q,CRC,ITL>* BFER_std
::build(const parameters<C> &params)
{
	return params.template build<B,R,Q,CRC,ITL>();
}

template <class C>
void BFER_std::store_args(const arg_val_map &vals, parameters<C> &params, const std::string p)
{
	BFER::store_args(vals, params, p);
}

template <class C>
void BFER_std::make_header(params_list& head_sim, const parameters<C> &params, const bool full)
{
	BFER::make_header(head_sim, params, full);
}
}
}
