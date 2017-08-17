#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFER_STD_HPP_
#define SC_SIMULATION_BFER_STD_HPP_

#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"

#include "../BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class SC_BFER_std : public BFER_std<B,R,Q>
{
protected:
	tools::SC_Duplicator *duplicator[3];
	tools::SC_Debug<B>   *dbg_B     [7];
	tools::SC_Debug<R>   *dbg_R     [5];
	tools::SC_Debug<Q>   *dbg_Q     [3];

public:
	SC_BFER_std(const factory::BFER_std::parameters &chain_params, tools::Codec<B,Q> &codec);
	virtual ~SC_BFER_std();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void bind_sockets      ();
	void bind_sockets_debug();
};
}
}

#endif /* SC_SIMULATION_BFER_STD_HPP_ */

#endif /* SYSTEMC */
