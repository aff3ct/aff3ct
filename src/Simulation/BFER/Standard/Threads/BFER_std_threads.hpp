#ifndef SIMULATION_BFER_STD_THREADS_HPP_
#define SIMULATION_BFER_STD_THREADS_HPP_

#include "../BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_std_threads : public BFER_std<B,R,Q>
{
public:
	BFER_std_threads(const factory::BFER_std::parameters &params);
	virtual ~BFER_std_threads();

protected:
	virtual void __build_communication_chain(const int tid = 0);
	virtual void _launch();

private:
	void simulation_loop(const int tid = 0);

	static void start_thread(BFER_std_threads<B,R,Q> *simu, const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_STD_THREADS_HPP_ */
