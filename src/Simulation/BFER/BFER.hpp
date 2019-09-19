#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <mutex>
#include <chrono>
#include <vector>
#include <memory>

#include "Factory/Simulation/BFER/BFER.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Module/Monitor/Monitor_reduction.hpp"
#ifdef AFF3CT_MPI
#include "Module/Monitor/Monitor_reduction_MPI.hpp"
#endif
#include "Simulation/Simulation.hpp"

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

	// code specifications
	const float bit_rate;

	std::unique_ptr<tools::Noise<R>> noise; // current simulated noise

	// the monitors of the the BFER simulation
	using Monitor_BFER_type = module::Monitor_BFER<B>;
	using Monitor_MI_type   = module::Monitor_MI<B,R>;

#ifdef AFF3CT_MPI
	using Monitor_BFER_reduction_type = module::Monitor_reduction_MPI<Monitor_BFER_type>;
	using Monitor_MI_reduction_type   = module::Monitor_reduction_MPI<Monitor_MI_type  >;
#else
	using Monitor_BFER_reduction_type = module::Monitor_reduction_M<Monitor_BFER_type>;
	using Monitor_MI_reduction_type   = module::Monitor_reduction_M<Monitor_MI_type  >;
#endif

	std::vector<std::unique_ptr<Monitor_MI_type          >> monitor_mi;
	            std::unique_ptr<Monitor_MI_reduction_type>  monitor_mi_red;

	std::vector<std::unique_ptr<Monitor_BFER_type          >> monitor_er;
	            std::unique_ptr<Monitor_BFER_reduction_type>  monitor_er_red;

	// dump frames into files
	std::vector<std::unique_ptr<tools::Dumper          >> dumper;
	            std::unique_ptr<tools::Dumper_reduction>  dumper_red;


	// terminal and reporters (for the output of the simu)
	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	            std::unique_ptr<tools::Terminal>  terminal;

	// noise distribution
	std::unique_ptr<tools::Distributions<R>> distributions;

	std::chrono::steady_clock::time_point t_start_noise_point;

public:
	explicit BFER(const factory::BFER::parameters& params_BFER);
	virtual ~BFER() = default;
	void launch();

protected:
	        void  _build_communication_chain();
	virtual void __build_communication_chain(const int tid = 0) = 0;
	virtual void _launch() = 0;

	std::unique_ptr<Monitor_MI_type>   build_monitor_mi(const int tid = 0);
	std::unique_ptr<Monitor_BFER_type> build_monitor_er(const int tid = 0);
	std::unique_ptr<tools::Terminal>   build_terminal();
	void build_reporters();
	void build_monitors ();

	virtual bool keep_looping_noise_point();
	bool stop_time_reached();

private:
	static void start_thread_build_comm_chain(BFER<B,R,Q> *simu, const int tid);
};
}
}

#endif /* SIMULATION_BFER_HPP_ */
