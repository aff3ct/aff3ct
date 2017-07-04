#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <map>
#include <chrono>
#include <vector>

#include "Tools/Threads/Barrier.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"
#include "Module/Monitor/Monitor.hpp"
#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "Tools/Codec/Codec.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_simulation_BFER.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER : public Simulation
{
private:
	// parameters
	const typename tools::Factory_simulation_BFER::chain_parameters_BFER<B,R,Q> &params;

	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	bool stop_terminal;

protected:
	std::mutex  mutex_exception;
	std::string prev_err_message;

	tools::Codec<B,Q> &codec;

	// a barrier to synchronize the threads
	tools::Barrier barrier;

	// code specifications
	float snr;
	float snr_s;
	float snr_b;
	float sigma;

	// the monitors of the the BFER simulation
	std::vector<module::Monitor          <B>*> monitor;
	            module::Monitor_reduction<B>*  monitor_red;

	// dump frames into files
	std::vector<tools::Dumper          *> dumper;
	            tools::Dumper_reduction*  dumper_red;

	// terminal (for the output of the code)
	tools::Terminal_BFER<B> *terminal;

	// time points and durations
	std::vector<std::map<std::pair<int, std::string>, std::chrono::nanoseconds>> durations;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_red;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_sum;

	// size of the data
	std::map<std::pair<int, std::string>, unsigned> data_sizes;

public:
	Simulation_BFER(const typename tools::Factory_simulation_BFER::chain_parameters_BFER<B,R,Q>& simu_params, tools::Codec<B,Q> &codec);
	virtual ~Simulation_BFER();
	void launch();

protected:
	virtual void _build_communication_chain(const int tid = 0);
	virtual void release_objects();
	virtual void _launch() = 0;

	        module::Monitor      <B>* build_monitor (const int tid = 0);
	virtual tools ::Terminal_BFER<B>* build_terminal(                 );

private:
	void build_communication_chain(const int tid = 0);
	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

	static void start_thread_terminal        (Simulation_BFER<B,R,Q> *simu               );
	static void start_thread_build_comm_chain(Simulation_BFER<B,R,Q> *simu, const int tid);
};
}
}

#endif /* SIMULATION_BFER_HPP_ */
