#include "Tools/Display/rang_format/rang_format.h"

#include "Command_parser.hpp"

using namespace aff3ct;
using namespace factory;

const tools::Argument_tag aff3ct::factory::Command_parser::help_tag = {"help", "h"};

Command_parser
::Command_parser(int argc, char** argv,
                 std::vector<Factory::parameters*> &_params,
	             bool add_help_tag,
                 std::ostream& _err_stream)
: ahandler(argc, (const char**)argv), params(_params), err_stream(_err_stream), help_asked(false)
{
	parse(add_help_tag);
}

Command_parser
::~Command_parser()
{

}

void Command_parser
::parse(bool add_help_tag)
{
	// build the required and optional arguments for the cmd line
	Factory::get_description(params, args);

	if (add_help_tag && !args.exist(Command_parser::help_tag))
	{
		args.add(Command_parser::help_tag, tools::None(), "print this help.");
	}

	// parse the argument from the command line
	auto read_args = ahandler.parse_arguments(args, warnings, errors);

	try
	{
		// write the parameters values in "params" from "read_args"
		Factory::store(params, read_args);
	}
	catch(const std::exception& e)
	{
		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		errors.push_back(e.what());
		tools::exception::no_backtrace = save;
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
		auto grps = Factory::create_groups(params);

		// display the command usage and the help (the parameters are ordered by group)
		ahandler.print_help(args, grps);

		std::cout << std::endl;
	}
	else if (args.exist(Command_parser::help_tag))
	{
		std::string message = "For more information please display the help (\"";
		message += tools::Argument_handler::print_tag(Command_parser::help_tag) += "\").";

		err_stream << rang::format::info << message << rang::format::reset << std::endl << std::endl;
	}
}

void Command_parser
::print_errors()
{
	for (size_t e = 0; e < errors.size(); e++)
		err_stream << rang::format::error << errors[e] << rang::format::reset << std::endl;
}

void Command_parser
::print_warnings()
{
	for (size_t w = 0; w < warnings.size(); w++)
		err_stream << rang::format::warning << warnings[w] << rang::format::reset << std::endl;
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
