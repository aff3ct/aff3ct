#ifdef SYSTEMC

#ifndef SC_SIMULATION_BFER_ITE_HPP_
#define SC_SIMULATION_BFER_ITE_HPP_

#include "Tools/SystemC/SC_Debug.hpp"
#include "Tools/SystemC/SC_Router.hpp"
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
	module::Interleaver<int> *interleaver_e;
	module::Coset<B,Q>       *coset_real_i;

	tools::SC_Duplicator *duplicator[5];
	tools::SC_Router     *router;
	tools::SC_Predicate  *predicate;

	tools::SC_Debug<B> *dbg_B[7];
	tools::SC_Debug<R> *dbg_R[4];
	tools::SC_Debug<Q> *dbg_Q[7];

public:
	SC_BFER_ite(const factory::BFER_ite::parameters &params, tools::Codec_SISO<B,Q> &codec);
	virtual ~SC_BFER_ite();

protected:
	virtual void __build_communication_chain(const int tid = 0);
	virtual void release_objects();
	virtual void _launch();

	virtual module::Interleaver<int>* build_interleaver(const int tid = 0, const int seed = 0);
	virtual module::Coset      <B,Q>* build_coset_real (const int tid = 0                    );

private:
	void bind_sockets      ();
	void bind_sockets_debug();
};
}
}

#endif /* SC_SIMULATION_BFER_ITE_HPP_ */

#endif /* SYSTEMC */
