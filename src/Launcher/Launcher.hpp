/*!
 * \file
 * \brief Collects the command line parameters and launches the simulation.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef LAUNCHER_HPP_
#define LAUNCHER_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Tools/Arguments/Argument_handler.hpp"
#include "Tools/Arguments/Maps/Argument_map_info.hpp"
#include "Tools/Arguments/Maps/Argument_map_value.hpp"
#include "Factory/Simulation/Simulation.hpp"
#include "Simulation/Simulation.hpp"

namespace aff3ct
{
namespace launcher
{
/*!
 * \class Launcher
 *
 * \brief Collects the command line parameters and launches the simulation.
 *        Describes and provides tools to make a working launcher.
 */
class Launcher
{
private:
	std::unique_ptr<simulation::Simulation> simu;     /*!< A generic simulation pointer to allocate a specific simulation. */
	std::string                             cmd_line;
	std::vector<std::string>                cmd_warn;

protected:
	tools::Argument_handler          ah;             /*!< An argument reader to manage the parsing and the documentation of the command line parameters. */
	tools::Argument_map_info         args;           /*!< List of the arguments to find in the command line */
	tools::Argument_map_value        arg_vals;       /*!< List of the arguments with their values */
	factory::Simulation::parameters &params_common;  /*!< A structure of parameters to store and pass to the simulation. */
	std::ostream                    &stream;         /*!< The dedicated stream in which the Launcher writes the parameters. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Takes the famous "argc" and "argv" arguments from the main function.
	 *
	 * \param argc:   number of arguments.
	 * \param argv:   array of arguments
	 * \param stream: the stream in which the Launcher writes the parameters.
	 */
	Launcher(const int argc, const char **argv, factory::Simulation::parameters &params_common,
	         std::ostream &stream = std::cout);

	/*!
	 * \brief Destructor.
	 *
	 * Deallocates the simulation.
	 */
	virtual ~Launcher() = default;

	/*!
	 * \brief Launch the simulation.
	 */
	int launch();

protected:
	/*!
	 * \brief Builds the various arguments required (or/and optional) for the simulation.
	 *
	 * This method can be overloaded to be extended.
	 */
	virtual void get_description_args();

	/*!
	 * \brief Stores the values from the command line to the internal parameters.
	 *
	 * This method can be overloaded to be extended.
	 */
	virtual void store_args();

	/*!
	 * \brief Allocates a specific simulation.
	 *
	 * This method have to be overloaded.
	 *
	 * \return a new simulation.
	 */
	virtual simulation::Simulation* build_simu() = 0;

	void print_header();

private:
	int read_arguments();
};
}
}

#endif /* LAUNCHER_HPP_ */
