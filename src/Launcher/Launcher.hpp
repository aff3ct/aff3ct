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
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "Tools/types.h"
#include "Tools/params.h"
#include "Tools/Arguments_reader.hpp"
#include "Simulation/Simulation.hpp"

/*!
 * \class Launcher
 *
 * \brief Collects the command line parameters and launches the simulation.
 *        Describes and provides tools to make a working launcher.
 *
 * \tparam B: type of the bits in the simulation.
 * \tparam R: type of the reals (floating-point representation) in the simulation.
 * \tparam Q: type of the quantified reals (fixed-point representation) in the simulation.
 */
template <typename B, typename R, typename Q>
class Launcher
{
private:
	int                                             max_n_chars; /*!< The number of characters of the largest parameter name. */
	std::unordered_map<std::type_index,std::string> type_names;  /*!< An internal map to store a string associated to a type. */
	Simulation                                     *simu;        /*!< A generic simulation pointer to allocate a specific simulation. */

protected:

	Arguments_reader                                             ar;       /*!< An argument reader to manage the parsing and the documentation of the command line parameters. */
	parameters                                                   params;   /*!< A structure of parameters to store and pass to the simulation. */
	std::ostream                                                &stream;   /*!< The dedicated stream in which the Launcher writes the parameters. */
	std::map<std::vector<std::string>, std::vector<std::string>> req_args; /*!< List of the required arguments, syntax is the following:
	                                                                        *!< req_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */
	std::map<std::vector<std::string>, std::vector<std::string>> opt_args; /*!< List of the optional arguments, syntax is the following:
	                                                                        *!< opt_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */


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
	 * \brief Returns a vector of simulation parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_simulation();

	/*!
	 * \brief Code parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_code();

	/*!
	 * \brief Returns a vector of source parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_source();

	/*!
	 * \brief Returns a vector of CRC parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_crc();

	/*!
	 * \brief Returns a vector of encoder parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_encoder();

	/*!
	 * \brief Returns a vector of puncturer parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_puncturer();

	/*!
	 * \brief Returns a vector of interleaver parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_interleaver();

	/*!
	 * \brief Returns a vector of modulator parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_modulator();

	/*!
	 * \brief Returns a vector of channel parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_channel();

	/*!
	 * \brief Returns a vector of demodulator parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_demodulator();

	/*!
	 * \brief Returns a vector of depuncturer parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_depuncturer();

	/*!
	 * \brief Returns a vector of quantizer parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_quantizer();

	/*!
	 * \brief Returns a vector of decoder parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_decoder();

	/*!
	 * \brief Returns a vector of monitor parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_monitor();

	/*!
	 * \brief Returns a vector of terminal parameters to display in the header of the simulation.
	 *
	 * This method can be overloaded to be extended.
	 *
	 * \return a vector of pair containing the parameters to display (pair.first = "Key", pair.second = "Value")
	 */
	virtual std::vector<std::pair<std::string,std::string>> header_terminal();

	/*!
	 * \brief Allocates a specific simulation.
	 *
	 * This method have to be overloaded.
	 *
	 * \return a new simulation.
	 */
	virtual Simulation* build_simu() = 0;

private:
	void read_arguments();
	void print_header();
	void print_parameters(std::string grp_name, std::vector<std::pair<std::string,std::string>> params);
	void compute_max_n_chars();
};

#endif /* LAUNCHER_HPP_ */
