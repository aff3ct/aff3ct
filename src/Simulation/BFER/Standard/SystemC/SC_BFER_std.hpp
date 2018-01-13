#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFER_STD_HPP_
#define SC_SIMULATION_BFER_STD_HPP_

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

public:
	explicit SC_BFER_std(const factory::BFER_std::parameters &params);
	virtual ~SC_BFER_std();

protected:
	void create_sc_modules();

	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void bind_sockets();
};
}
}

#endif /* SC_SIMULATION_BFER_STD_HPP_ */

#endif /* SYSTEMC */
