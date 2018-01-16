#include "Command_scanner.hpp"

using namespace aff3ct;
using namespace factory;

const tools::Argument_tag aff3ct::factory::Command_scanner::help_tag = {"help", "h"};

Command_scanner
::Command_scanner(int argc, char** argv,
                 std::vector<Factory::parameters*> &_params,
	             bool add_help_tag,
                 std::ostream& _err_stream)
: ahandler(argc, (const char**)argv), params(_params), err_stream(_err_stream), help_asked(false)
{
	parse(add_help_tag);
}

Command_scanner
::~Command_scanner()
{

}

void Command_scanner
::parse(bool add_help_tag)
{
	// build the required and optional arguments for the cmd line
	Factory::get_description(params, req_args, opt_args);

	if (add_help_tag && !opt_args.exist(Command_scanner::help_tag))
	{
		opt_args.add(Command_scanner::help_tag, tools::None(), "print this help.");
	}

	// parse the argument from the command line
	auto read_args = ahandler.parse_arguments(req_args, opt_args, warnings, errors);

	try
	{
		// write the parameters values in "params" from "read_args"
		Factory::store(params, read_args);
	}
	catch(const aff3ct::tools::exception& e)
	{
		errors.push_back(e.what_no_bt());
	}
	catch(const std::exception& e)
	{
		errors.push_back(tools::addr2line(e.what()));
	}

	if (read_args.exist(Command_scanner::help_tag))
		help_asked = true;
}

void Command_scanner
::print_help()
{
	if (help_asked)
	{
		// create groups of arguments
		auto grps = Factory::create_groups(params);

		// display the command usage and the help (the parameters are ordered by group)
		ahandler.print_help(req_args, opt_args, grps);

		std::cout << std::endl;
	}
	else if (opt_args.exist(Command_scanner::help_tag))
	{
		std::string message = "For more information please display the help (\"";
		message += tools::Argument_handler::print_tag(Command_scanner::help_tag) += "\").";

		err_stream << tools::format_info(message) << std::endl << std::endl;
	}
}

void Command_scanner
::print_errors()
{
	for (size_t e = 0; e < errors.size(); e++)
		err_stream << aff3ct::tools::format_error(errors[e]) << std::endl;
}

void Command_scanner
::print_warnings()
{
	for (size_t w = 0; w < warnings.size(); w++)
		err_stream << aff3ct::tools::format_warning(warnings[w]) << std::endl;
}

bool Command_scanner
::parsing_failed()
{
	return errors.size() > 0;
}

bool Command_scanner
::parsing_succeeded()
{
	return errors.empty();
}
