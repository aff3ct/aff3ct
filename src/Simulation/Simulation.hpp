/*!
 * \file
 * \brief The Simulation is an abstract and generic container.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <stdexcept>

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
public:
	/*!
	 *  \brief Constructor.
	 */
	Simulation()
	{
	}

	/*!
	 *  \brief Destructor.
	 */
	virtual ~Simulation()
	{
	}

	/*!
	 *  \brief This method has to be overloaded, this is the start point of the Simulation.
	 */
	virtual void launch() = 0;
};
}
}

#endif /* SIMULATION_HPP_ */
