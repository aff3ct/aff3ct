#ifndef SIMULATION_BFER_ITE_THREADS_HPP_
#define SIMULATION_BFER_ITE_THREADS_HPP_

#include "../BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B = int, typename R = float, typename Q = R, int CRC = 0>
class BFER_ite_threads : public BFER_ite<C,B,R,Q,CRC>
{
public:
	BFER_ite_threads(const factory::BFER_ite::parameters<C> &params);
	virtual ~BFER_ite_threads();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void simulation_loop(const int tid = 0);

	static void start_thread(BFER_ite_threads<C,B,R,Q,CRC> *simu, const int tid = 0);
};
}
}

#include "BFER_ite_threads.hxx"

#endif /* SIMULATION_BFER_ITE_THREADS_HPP_ */
