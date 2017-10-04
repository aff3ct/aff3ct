#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFER_ITE_HPP_
#define SC_SIMULATION_BFER_ITE_HPP_

#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Router.hpp"
#include "Tools/SystemC/SC_Funnel.hpp"
#include "Tools/SystemC/SC_Predicate.hpp"
#include "Tools/SystemC/SC_Duplicator.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"

#include "../BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class SC_BFER_ite : public BFER_ite<B,R,Q>
{
protected:
	module::Coset<B,Q> *coset_real_i;

	tools::SC_Duplicator *duplicator[7];
	tools::SC_Router     *router;
	tools::SC_Funnel     *funnel;
	tools::SC_Predicate  *predicate;

public:
	SC_BFER_ite(const factory::BFER_ite::parameters &params);
	virtual ~SC_BFER_ite();

protected:
	void create_sc_modules();
	void erase_sc_modules();

	virtual void __build_communication_chain(const int tid = 0);
	virtual void release_objects();
	virtual void _launch();

	virtual module::Coset<B,Q>* build_coset_real(const int tid = 0);

private:
	void bind_sockets      ();
	void bind_sockets_debug();
};
}
}

#endif /* SC_SIMULATION_BFER_ITE_HPP_ */

#endif /* SYSTEMC */
