/*!
 * \file
 * \brief Manages the command line arguments.
 * \date  2014
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ARGUMENTS_READER_HPP_
#define ARGUMENTS_READER_HPP_

#include <string>
#include <vector>
#include <map>

namespace aff3ct
{
namespace tools
{
/*!
 * \class Arguments_reader
 * \brief Eases the management of the arguments from the command line.
 */
class Arguments_reader
{
public:
	using arg_map = std::map<std::vector<std::string>, std::vector<std::string>>;
	using arg_grp = std::vector<std::vector<std::string>>;

private:
	std::vector<std::string>                        m_argv;          /*!< Copy of "char** argv". */
	arg_map                                         m_required_args; /*!< List of the required arguments, syntax is the following:
	                                                                  *!< m_required_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */
	arg_map                                         m_optional_args; /*!< List of the optional arguments, storage is made as following:
	                                                                  *!< m_optional_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}. */
	std::map<std::vector<std::string>, std::string> m_args;          /*!< List of the arguments and associate values (after parsing), storage is made as following:
	                                                                  *!< m_args[{"key1", "key2", [...]}] = "value". */
	std::string                                     m_program_name;  /*!< Program binary name. */
	unsigned int                                    max_n_char_arg;  /*!< The number of characters of the largest argument name. */

public:
	/*!
	 * \brief Constructor.
	 *
	 * Takes the famous "argc" and "argv" arguments from the main function.
	 *
	 * \param argc: number of arguments.
	 * \param argv: array of arguments
	 */
	Arguments_reader(const int argc, const char** argv);

	/*!
	 * \brief Destructor.
	 *
	 * Actually do nothing...
	 */
	virtual ~Arguments_reader();

	/*!
	 * \brief Parse "m_argv".
	 *
	 * Parses "m_argv" following the given required and optional arguments.
	 *
	 * \param required_args: map of the required arguments,
	 *                       required_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 * \param optional_args: map of the optional arguments,
	 *                       optional_args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 *
	 * \return true if all the required arguments are in "m_argv", false otherwise.
	 */
	bool parse_arguments(const arg_map &required_args, const arg_map &optional_args,
	                     const bool display_warnings = false);

	bool parse_arguments(const arg_map &required_args, const arg_map &optional_args,
	                     std::vector<std::string> &warnings);

	/*!
	 * \brief Searches if the tags exist.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return true if the argument exists (to use after the parse_arguments method).
	 */
	bool exist_argument(const std::vector<std::string> &tags) const;

	/*!
	 * \brief Alias of the exist_argument method.
	 *
	 * \copydoc Arguments_reader::exist_argument
	 */
	bool exist_arg(const std::vector<std::string> &tags) const
	{
		return exist_argument(tags);
	}

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the string value of an argument with its tags (to use after the parse_arguments method).
	 */
	std::string get_argument(const std::vector<std::string> &tags) const;

	/*!
	 * \brief Alias of the Arguments_reader::get_argument method.
	 *
	 * \copydoc Arguments_reader::get_argument
	 */
	std::string get_arg(const std::vector<std::string> &tags) const
	{
		return get_argument(tags);
	}

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}
	 *
	 * \return the integer value of an argument with its tags (to use after the parse_arguments method).
	 */
	int get_arg_int(const std::vector<std::string> &tags) const;

	/*!
	 * \brief Returns the value for an argument.
	 *
	 * \param tags: list of tags associated to an argument, tags = {"Key1", "Key2", [...]}.
	 *
	 * \return the floating-point value of an argument with its tags (to use after the parse_arguments method).
	 */
	float get_arg_float(const std::vector<std::string> &tags) const;

	/*!
	 * \brief Prints the traditional usage.
	 */
	void print_usage() const;

	/*!
	 * \brief Prints the traditional usage and group the arguments.
	 *
	 * \param arg_groups: group of argument based on a prefix,
	 *                    arg_groups = {{"prefix1", "Group name1"}, {"prefix2", "Group name2"}, [...]}.
	 */
	void print_usage(arg_grp arg_groups) const;

	/*!
	 * \brief Checks if the values from the command line respect the criteria given by required_args and optional_args.
	 *
	 * \param error: error message.
	 *
	 * \return true if the arguments criteria are respected, false otherwise.
	 */
	bool check_arguments(std::vector<std::string> &error) const;

	/*
	 * return the given tag with its command line argument format.
	 * Add "-" before if tag is one letter length else "--".
	 */
	static std::string print_tag(const std::string& tag);

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
	bool sub_parse_arguments(arg_map &args, unsigned short pos_arg);

	/*!
	 * \brief Checks if the values from the command line respect the criteria given by required_args and optional_args
	 *        (this is for one argument only).
	 *
	 * \param tags: tags of an argument, tags = {"Key1", "Key2", [...]}.
	 * \param args: map of arguments,
	 *              args[{"key1", "key2", [...]}] = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 *
	 * \return true if the argument criteria are respected, false otherwise.
	 */
	std::string check_argument(const std::vector<std::string> &tags, const arg_map &args) const;

	/*!
	 * \brief Clears m_required_args, m_optional_args and m_args.
	 */
	void clear_arguments();

	/*!
	 * \brief Prints usage for one argument.
	 *
	 * \param tags:     tags of an argument, tags = {"Key1", "Key2", [...]}.
	 * \param values:   the type, doc and possible choices for an argument,
	 *                  values = {"type", ["doc"], ["possible choices separated by a comma"]}.
	 * \param required: true if this is a required parameter.
	 */
	void print_usage(const std::vector<std::string> &tags, const std::vector<std::string> &values,
	                 const bool required = false) const;

	/*!
	 * \brief Splits a string in a vector of string, the delimiter is the comma.
	 *
	 * \param str: the string to be split.
	 *
	 * \return A vector of split strings.
	 */
	std::vector<std::string> split(std::string str) const;
};
}
}

#endif /* ARGUMENTS_READER_HPP_ */
