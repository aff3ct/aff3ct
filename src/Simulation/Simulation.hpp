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

protected:
	/*!
	 * \brief Checks if a pointer has been allocated or not, if not the program stop with an error message.
	 *
	 * \param ptr: a pointer.
	 * \param str: name of the pointer.
	 * \param tid: thread identifier.
	 */
	static void check_errors(void *ptr, std::string str, const int tid = 0)
	{
		if (ptr == nullptr)
			throw std::runtime_error("aff3ct::simulation::Simulation: something went wrong during the "
			                         "\"" + str + "\" instantiation (tid = " + std::to_string(tid) + ").");
	}
};
}
}

#endif /* SIMULATION_HPP_ */
