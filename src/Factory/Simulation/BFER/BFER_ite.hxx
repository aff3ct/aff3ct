#include "Simulation/BFER/Iterative/SystemC/SC_BFER_ite.hpp"
#include "Simulation/BFER/Iterative/Threads/BFER_ite_threads.hpp"

#include "BFER_ite.hpp"

namespace aff3ct
{
namespace factory
{
template <class C>
template <typename B, typename R, typename Q, int CRC>
simulation::BFER_ite<C,B,R,Q,CRC>* BFER_ite::parameters<C>
::build() const
{
#if defined(SYSTEMC)
	return new simulation::SC_BFER_ite     <C,B,R,Q,CRC>(*this);
#else
	return new simulation::BFER_ite_threads<C,B,R,Q,CRC>(*this);
#endif
}

template <class C, typename B, typename R, typename Q, int CRC>
simulation::BFER_ite<C,B,R,Q,CRC>* BFER_ite
::build(const parameters<C> &params)
{
	return params.template build<B,R,Q,CRC>();
}

template <class C>
void BFER_ite::store_args(const arg_val_map &vals, parameters<C> &params, const std::string p)
{
	BFER::store_args(vals, params, p);

	if(exist(vals, {p+"-ite", "I"})) params.n_ite = std::stoi(vals.at({p+"-ite", "I"}));

	if(params.err_track_revert)
	{
		params.itl.core.type = "USER";
		params.itl.core.path = params.err_track_path + std::string("_$snr.itl");
	}
}

template <class C>
void BFER_ite::make_header(params_list& head_sim, const parameters<C> &params, const bool full)
{
	BFER::make_header(head_sim, params, full);

	head_sim.push_back(std::make_pair("Global iterations (I)", std::to_string(params.n_ite)));
}
}
}
