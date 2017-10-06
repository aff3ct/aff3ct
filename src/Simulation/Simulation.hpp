/*!
 * \file
 * \brief The Simulation is an abstract and generic container.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

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
protected:
	// parameters
	const factory::Simulation::parameters &params;

	// map of Modules
	std::map<std::string, std::vector<module::Module*>> modules;

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
	void build_communication_chain();
	virtual void _build_communication_chain() = 0;
};
}
}

#endif /* SIMULATION_HPP_ */
