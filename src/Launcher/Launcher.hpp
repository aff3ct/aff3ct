/*!
 * \file
 * \brief Collects the command line parameters and launches the simulation.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef LAUNCHER_HPP_
#define LAUNCHER_HPP_

#include <map>
#include <string>

#include "Tools/types.h"
#include "Tools/Arguments_reader.hpp"

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
	simulation::Simulation *simu;               /*!< A generic simulation pointer to allocate a specific simulation. */
	std::string             cmd_line;

protected:
	tools::Arguments_reader          ar;        /*!< An argument reader to manage the parsing and the documentation of the command line parameters. */
	tools::Arguments_reader::arg_map req_args;  /*!< List of the required arguments, syntax is the following:
	                                             *!< req_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */
	tools::Arguments_reader::arg_map opt_args;  /*!< List of the optional arguments, syntax is the following:
	                                             *!< opt_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */
	tools::Arguments_reader::arg_grp arg_group; /*!< List of the arguments groups */

	factory::Simulation::parameters *params;    /*!< A structure of parameters to store and pass to the simulation. */

	std::ostream &stream;                       /*!< The dedicated stream in which the Launcher writes the parameters. */
	factory::params_list pl_sim, pl_src, pl_crc, pl_itl, pl_mdm, pl_chn,
	                     pl_qnt, pl_enc, pl_dec, pl_mnt, pl_ter, pl_pct;

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
	Launcher(const int argc, const char **argv, std::ostream &stream = std::cout);

	/*!
	 * \brief Destructor.
	 *
	 * Deallocates the simulation.
	 */
	virtual ~Launcher();

	/*!
	 * \brief Launch the simulation.
	 */
	void launch();

protected:
	/*!
	 * \brief Builds the various arguments required (or/and optional) for the simulation.
	 *
	 * This method can be overloaded to be extended.
	 */
	virtual void build_args();

	/*!
	 * \brief Stores the values from the command line to the internal parameters.
	 *
	 * This method can be overloaded to be extended.
	 */
	virtual void store_args();

	/*!
	 * \brief Gathers the arguments in separated groups
	 *
	 * This method can be overloaded to be extended.
	 */
	virtual void group_args();

	/*!
	 * \brief Allocates a specific simulation.
	 *
	 * This method have to be overloaded.
	 *
	 * \return a new simulation.
	 */
	virtual simulation::Simulation* build_simu() = 0;

	virtual void print_header();

private:
	int read_arguments();
	void compute_max_n_chars();
};
}
}

#endif /* LAUNCHER_HPP_ */
