#include <stdexcept>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Command_parser.hpp"

using namespace aff3ct;
using namespace tools;

const cli::Argument_tag aff3ct::tools::Command_parser::help_tag = {"help", "h"};

Command_parser
::Command_parser(int argc, char** argv,
                 std::vector<factory::Factory*> &_factories,
	             bool add_help_tag,
                 std::ostream& _err_stream)
: ahandler(argc, (const char**)argv), factories(_factories), err_stream(_err_stream), help_asked(false)
{
	parse(add_help_tag);
}

void Command_parser
::parse(bool add_help_tag)
{
	// build the required and optional arguments for the cmd line
	factory::Factory::get_description(factories, args);

	if (add_help_tag && !args.exist(Command_parser::help_tag))
	{
		args.add(Command_parser::help_tag, cli::None(), "print this help.");
	}

	// parse the argument from the command line
	auto read_args = ahandler.parse_arguments(args, warnings, errors);

	try
	{
		// write the parameters values in "factories" from "read_args"
		factory::Factory::store(factories, read_args);
	}
	catch(const std::exception& e)
	{
		auto save = tools::exception::no_stacktrace;
		tools::exception::no_stacktrace = true;
		errors.push_back(e.what());
		tools::exception::no_stacktrace = save;
	}

	if (read_args.exist(Command_parser::help_tag))
		help_asked = true;
}

void Command_parser
::print_help()
{
	if (help_asked)
	{
		// create groups of arguments
		auto grps = factory::Factory::create_groups(factories);

		// display the command usage and the help (the parameters are ordered by group)
		ahandler.print_help(args, grps);

		std::cout << std::endl;
	}
	else if (args.exist(Command_parser::help_tag))
	{
		std::string message = "For more information please display the help (\"";
		message += cli::Argument_handler::print_tag(Command_parser::help_tag) += "\").";

		err_stream << rang::tag::info << message << std::endl << std::endl;
	}
}

void Command_parser
::print_errors()
{
	for (size_t e = 0; e < errors.size(); e++)
		err_stream << rang::tag::error << errors[e] << std::endl;
}

void Command_parser
::print_warnings()
{
	for (size_t w = 0; w < warnings.size(); w++)
		err_stream << rang::tag::warning << warnings[w] << std::endl;
}

bool Command_parser
::help_required()
{
	return help_asked;
}

bool Command_parser
::parsing_failed()
{
	return errors.size() > 0;
}

bool Command_parser
::parsing_succeeded()
{
	return errors.empty();
}
