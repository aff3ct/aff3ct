#ifndef SIMULATION_BFER_STD_THREADS_HPP_
#define SIMULATION_BFER_STD_THREADS_HPP_

#include "../BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B = int, typename R = float, typename Q = R, int CRC = 0, int ITL = 0>
class BFER_std_threads : public BFER_std<C,B,R,Q,CRC,ITL>
{
public:
	BFER_std_threads(const factory::BFER_std::parameters<C> &params);
	virtual ~BFER_std_threads();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void simulation_loop(const int tid = 0);

	static void start_thread(BFER_std_threads<C,B,R,Q,CRC,ITL> *simu, const int tid = 0);
};
}
}

#include "BFER_std_threads.hxx"

#endif /* SIMULATION_BFER_STD_THREADS_HPP_ */
