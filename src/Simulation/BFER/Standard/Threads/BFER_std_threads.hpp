#ifndef SIMULATION_BFER_STD_THREADS_HPP_
#define SIMULATION_BFER_STD_THREADS_HPP_

#include <vector>
#include <thread>
#include <mutex>
#include <mipp.h>

#include "../BFER_std.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_std_threads : public BFER_std<B,R,Q>
{
protected:
	std::map<std::thread::id, int> thread_id;

	std::mutex mutex_debug;

public:
	BFER_std_threads(const factory::BFER_std::parameters &params, tools::Codec<B,Q> &codec);
	virtual ~BFER_std_threads();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void _launch();
	virtual tools::Terminal_BFER<B>* build_terminal();

private:
	void Monte_Carlo_method   (const int tid = 0);
	void simulation_loop      (const int tid = 0);
	void simulation_loop_bench(const int tid = 0);

	void display_debug();

	static void start_thread(BFER_std_threads<B,R,Q> *simu, const int tid = 0);
};
}
}

#endif /* SIMULATION_BFER_STD_THREADS_HPP_ */
