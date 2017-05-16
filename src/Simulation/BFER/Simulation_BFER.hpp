#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <map>
#include <chrono>
#include <vector>

#include "Tools/params.h"
#include "Tools/Threads/Barrier.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Frame_dumper/Frame_dumper.hpp"
#include "Tools/Display/Frame_dumper/Frame_dumper_reduction.hpp"
#include "Module/Monitor/Monitor.hpp"
#include "Module/Monitor/Standard/Monitor_reduction.hpp"

#include "Tools/Codec/Codec.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R>
class Simulation_BFER : public Simulation
{
private:
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;

protected:
	tools::Codec<B,Q> &codec;

	// simulation parameters
	const tools::parameters &params;

	// a barrier to synchronize the threads
	tools::Barrier barrier;

	// code specifications
	float snr;
	float snr_s;
	float snr_b;
	float code_rate;
	float sigma;

	// the monitors of the the BFER simulation
	std::vector<module::Monitor          <B>*> monitor;
	            module::Monitor_reduction<B>*  monitor_red;

	// dump frames into files
	std::vector<tools::Frame_dumper          <B,R>*> dumper;
	            tools::Frame_dumper_reduction<B,R>*  dumper_red;

	// terminal (for the output of the code)
	tools::Terminal *terminal;

	// time points and durations
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> t_snr;

	std::vector<std::map<std::pair<int, std::string>, std::chrono::nanoseconds>> durations;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_red;
	            std::map<std::pair<int, std::string>, std::chrono::nanoseconds>  durations_sum;

public:
	Simulation_BFER(const tools::parameters& params, tools::Codec<B,Q> &codec);
	virtual ~Simulation_BFER();
	void launch();

protected:
	virtual void build_communication_chain(const int tid = 0);
	virtual void release_objects();
	virtual void _launch() = 0;

	virtual module::Monitor    <B  >* build_monitor (const int tid = 0);
	virtual tools::Frame_dumper<B,R>* build_dumper  (const int tid = 0);
	virtual tools::Terminal         * build_terminal(                 );

private:
	void time_reduction(const bool is_snr_done = false  );
	void time_report   (std::ostream &stream = std::clog);

	static void start_thread_terminal        (Simulation_BFER<B,R,Q> *simu               );
	static void start_thread_build_comm_chain(Simulation_BFER<B,R,Q> *simu, const int tid);
};
}
}

#endif /* SIMULATION_BFER_HPP_ */
