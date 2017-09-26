/*!
 * \file
 * \brief The Simulation is an abstract and generic container.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "Module/Module.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
/*!
 * \class Launcher
 *
 * \brief The Simulation is an abstract and generic container.
 */
class Simulation
{
private:
	std::thread term_thread;
	std::mutex mutex_terminal;
	std::condition_variable cond_terminal;
	bool stop_terminal;

protected:
	// parameters
	const factory::Simulation::parameters &params;

	// map of Modules
	std::map<std::string, std::vector<module::Module*>> modules;

	// terminal (for the output of the code)
	tools::Terminal *terminal;

public:
	/*!
	 *  \brief Constructor.
	 */
	Simulation(const factory::Simulation::parameters& simu_params);

	/*!
	 *  \brief Destructor.
	 */
	virtual ~Simulation();

	/*!
	 *  \brief This method has to be overloaded, this is the start point of the Simulation.
	 */
	virtual void launch() = 0;

protected:
	void display_stats(std::ostream &stream = std::cout);

	void build_communication_chain();

	virtual void _build_communication_chain() = 0;

	void start_terminal_temp_report(const std::chrono::milliseconds &freq);
	void stop_terminal_temp_report();

private:
	static void start_thread_terminal(Simulation *simu, const std::chrono::milliseconds &freq);
};
}
}

#endif /* SIMULATION_HPP_ */
