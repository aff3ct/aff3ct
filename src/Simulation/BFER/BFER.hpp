#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <map>
#include <chrono>
#include <vector>

#include "Tools/Threads/Barrier.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"
#include "../../Tools/Noise/Noise.hpp"

#include "Module/Module.hpp"
#include "Module/Monitor/Monitor.hpp"
#include "Module/Monitor/BFER/Monitor_BFER_reduction.hpp"

#include "Factory/Simulation/BFER/BFER.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class BFER : public Simulation
{
private:
	// parameters
	const factory::BFER::parameters &params_BFER;

protected:
	std::mutex               mutex_exception;
	std::vector<std::string> prev_err_messages;
	std::vector<std::string> prev_err_messages_to_display;

	// a barrier to synchronize the threads
	tools::Barrier barrier;

	// code specifications
	const float bit_rate;

	tools::Noise<R>* noise; // current noise simulated

	// the monitors of the the BFER simulation
	std::vector<module::Monitor_BFER          <B>*> monitor;
	            module::Monitor_BFER_reduction<B>*  monitor_red;

	// dump frames into files
	std::vector<tools::Dumper          *> dumper;
	            tools::Dumper_reduction*  dumper_red;

	// terminal (for the output of the code)
	tools::Terminal_BFER<B> *terminal;

public:
	explicit BFER(const factory::BFER::parameters& params_BFER);
	virtual ~BFER();
	void launch();

protected:
	        void  _build_communication_chain();
	virtual void __build_communication_chain(const int tid = 0) = 0;
	virtual void release_objects();
	virtual void _launch() = 0;

	module::Monitor_BFER <B>* build_monitor (const int tid = 0);
	tools ::Terminal_BFER<B>* build_terminal(                 );

private:
	static void start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid);
};
}
}

#endif /* SIMULATION_BFER_HPP_ */
