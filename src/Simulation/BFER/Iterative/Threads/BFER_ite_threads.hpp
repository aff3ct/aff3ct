#ifndef SIMULATION_BFER_ITE_THREADS_HPP_
#define SIMULATION_BFER_ITE_THREADS_HPP_

#include "../BFER_ite.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_ite_threads : public BFER_ite<B,R,Q>
{
public:
	explicit BFER_ite_threads(const factory::BFER_ite::parameters &params_BFER_ite);
	virtual ~BFER_ite_threads() = default;

protected:
	virtual void _launch();

private:
	void sockets_binding(const int tid = 0);
	void simulation_loop(const int tid = 0);

	static void start_thread(BFER_ite_threads<B,R,Q> *simu, const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_ITE_THREADS_HPP_ */
