/*!
 * \file
 * \brief Manages the command line arguments.
 * \date  2014
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ARGUMENT_HANDLER_HPP_
#define ARGUMENT_HANDLER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "Maps/Argument_map_info.hpp"
#include "Maps/Argument_map_group.hpp"
#include "Maps/Argument_map_value.hpp"

namespace aff3ct
{
namespace tools
{

/*!
 * \class Argument_handler
 * \brief Eases the management of the arguments from the command line.
 */
class Argument_handler
{
private:
	std::vector<std::string> command;            /*!< The argument list from the command line "char** argv". */
	std::string              m_program_name;     /*!< Program binary name. */
	std::ostream&            help_os;            /*!< The output stream on which will be displayed the help */
	bool                     help_display_keys;  /*!< Display the argument keys in the help */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Takes the famous "argc" and "argv" arguments from the main function.
	 *
	 * \param argc: number of arguments.
	 * \param argv: array of arguments
	 * \param help_os: the output stream on which is displayed the help
	 */
	Argument_handler(const int argc, const char** argv, std::ostream& help_os = std::cout);

	/*!
	 * \brief Destructor.
	 *
	 * Actually do nothing...
	 */
	virtual ~Argument_handler();

	/*!
	 * \brief Parse "m_argv" to check if all required arguments are present or if there are unknown ones
	 *
	 * \param warnings: warning messages.
	 *
	 * \return true if all the required arguments are in "m_argv", false otherwise.
	 */
	Argument_map_value parse_arguments(const Argument_map_info &args,
	                                   std::vector<std::string> &warnings, std::vector<std::string> &errors);

	void set_help_display_keys(const bool display);

	/*!
	 * \brief Checks if the values from the command line respect the criteria given by required_args and optional_args.
	 *
	 * \param error: error messages.
	 *
	 * \return true if the arguments criteria are respected, false otherwise.
	 */
	bool check_arguments(std::vector<std::string> &error) const;

	/*!
	 * \brief Prints the traditional usage with only the required arguments.
	 */
	void print_usage(const Argument_map_info &args) const;

	/*!
	 * \brief Prints the traditional usage and the whole help for arguments with rank lower or equal to 'print_level'
	 */
	void print_help(const Argument_map_info &args, bool print_advanced_args = true) const;

	/*!
	 * \brief Prints the traditional usage and the whole help grouped by argument categories.
	 *
	 * \param arg_groups: group of argument based on a prefix,
	 *                    arg_groups = {{"prefix1", "Group name1"}, {"prefix2", "Group name2"}, [...]}.
	 */
	void print_help(const Argument_map_info & args, const Argument_map_group& arg_groups,
	                bool print_advanced_args = true) const;

	/*
	 * return the given tag with its command line argument format.
	 * Add "-" before if tag is one letter length else "--".
	 */
	static std::string print_tag(const std::string& tag);

	/*
	 * return the given full tag with its command line argument format.
	 * Add "-" before if tag is one letter length else "--".
	 */
	static std::string print_tag(const Argument_tag &tags);

private:
	/*!
	 * \brief Returns true if the argument "m_argv[pos_arg]" is in args.
	 *
	 * \param args:    map of arguments,
	 *                 args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 * \param pos_arg: the position of the argument we are looking for (in m_argv[pos_arg]).
	 *
	 * \return true if the argument "m_argv[pos_arg]" is in args.
	 */
	std::vector<bool> sub_parse_arguments(const Argument_map_info &args, Argument_map_value& arg_v,
	                                      std::vector<bool>& command_found_pos, std::vector<std::string>& messages);

	/*!
	 * \brief Prints help for one argument.
	 *
	 * \param tags:     tags of an argument, tags = {"Key1", "Key2", [...]}.
	 * \param values:   the type, doc and possible choices for an argument,
	 *                  values = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 * \param required: true if this is a required parameter.
	 */
	void print_help(const Argument_tag &tags, const Argument_info &info, const size_t longest_tag) const;

	void print_help_title(const std::string& title) const;

	size_t find_longest_tags(const Argument_map_info &args) const;

	bool is_linked(const Argument_map_info &args, const Argument_map_value& arg_v, const Argument_tag &tag) const;
};
}
}

#endif /* ARGUMENT_HANDLER_HPP_ */
