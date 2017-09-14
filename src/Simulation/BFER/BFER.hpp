#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <map>
#include <chrono>
#include <vector>

#include "Tools/Threads/Barrier.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"

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
	const factory::BFER::parameters &params;

	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	bool stop_terminal;

protected:
	std::mutex  mutex_exception;
	std::string prev_err_message;

	// a barrier to synchronize the threads
	tools::Barrier barrier;

	const float bit_rate;

	// code specifications
	float snr;
	float snr_s;
	float snr_b;
	float sigma;

	unsigned max_fra;

	// map of Modules
	std::map<std::string, std::vector<module::Module*>> modules;

	// the monitors of the the BFER simulation
	std::vector<module::Monitor_BFER          <B>*> monitor;
	            module::Monitor_BFER_reduction<B>*  monitor_red;

	// dump frames into files
	std::vector<tools::Dumper          *> dumper;
	            tools::Dumper_reduction*  dumper_red;

	// terminal (for the output of the code)
	tools::Terminal_BFER<B> *terminal;

public:
	BFER(const factory::BFER::parameters& simu_params);
	virtual ~BFER();
	void launch();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void release_objects();
	virtual void _launch() = 0;

	module::Monitor_BFER <B>* build_monitor (const int tid = 0);
	tools ::Terminal_BFER<B>* build_terminal(                 );

private:
	void build_communication_chain(const int tid = 0);
	void display_stats(std::ostream &stream = std::cout);

	static void start_thread_terminal        (BFER<B,R,Q> *simu               );
	static void start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid);
};
}
}

#endif /* SIMULATION_BFER_HPP_ */
