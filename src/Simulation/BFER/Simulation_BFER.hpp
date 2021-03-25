#ifndef SIMULATION_BFER_HPP_
#define SIMULATION_BFER_HPP_

#include <vector>
#include <memory>
#include <chrono>

#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Tools/Sequence/Sequence.hpp"
#include "Tools/Constellation/Constellation.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/Display/Dumper/Dumper.hpp"
#include "Tools/Display/Dumper/Dumper_reduction.hpp"
#include "Tools/Monitor/Monitor_reduction.hpp"
#ifdef AFF3CT_MPI
#include "Tools/Monitor/Monitor_reduction_MPI.hpp"
#endif
#include "Tools/Noise/Noise.hpp"
#include "Factory/Simulation/Simulation.hpp"
#include "Factory/Simulation/BFER/BFER.hpp"
#include "Simulation/Simulation.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename B = int, typename R = float>
class Simulation_BFER : public Simulation
{
protected:
	const factory::Simulation &params;
	const factory::BFER       &params_BFER;

	std::unique_ptr<tools::Noise        < >> noise;
	std::vector    <float                  > channel_params;
	std::unique_ptr<tools::Distributions<R>> distributions;
	std::unique_ptr<tools::Constellation<R>> constellation;

	// communication sequence
	std::unique_ptr<module::Monitor_BFER<B    >> monitor_er;
	std::unique_ptr<module::Monitor_MI  <B,R  >> monitor_mi;
	std::unique_ptr<tools ::Sequence           > sequence;

	std::vector<std::unique_ptr<tools::Reporter>> reporters;
	std::unique_ptr<tools::Terminal> terminal;

#ifdef AFF3CT_MPI
	std::unique_ptr<tools::Monitor_reduction_MPI<module::Monitor_BFER<B>>> monitor_er_red;
	std::unique_ptr<tools::Monitor_reduction_MPI<module::Monitor_MI<B,R>>> monitor_mi_red;
#else
	std::unique_ptr<tools::Monitor_reduction<module::Monitor_BFER<B>>> monitor_er_red;
	std::unique_ptr<tools::Monitor_reduction<module::Monitor_MI<B,R>>> monitor_mi_red;
#endif

	// dump frames into files
	std::vector<std::unique_ptr<tools::Dumper         >> dumper;
	            std::unique_ptr<tools::Dumper_reduction> dumper_red;

	std::chrono::steady_clock::time_point t_start_noise_point;

public:
	explicit Simulation_BFER(const factory::BFER &params_BFER);

	virtual ~Simulation_BFER() = default;

	void launch();

protected:
	std::unique_ptr<module::Monitor_MI<B,R>> build_monitor_mi();
	std::unique_ptr<module::Monitor_BFER<B>> build_monitor_er();
	std::unique_ptr<tools ::Terminal> build_terminal(const std::vector<std::unique_ptr<tools::Reporter>> &reporters);
	std::vector<std::unique_ptr<tools::Reporter>> build_reporters(const tools ::Noise       < > *noise,
	                                                              const module::Monitor_BFER<B> *monitor_er,
	                                                              const module::Monitor_MI<B,R> *monitor_mi);

	virtual void create_modules();
	virtual void bind_sockets() = 0;
	virtual void create_sequence() = 0;
	void configure_sequence_tasks();
	void create_monitors_reduction();

	bool stop_time_reached();
	bool stop_condition();
};

}
}

#endif /* SIMULATION_BFER_HPP_ */
