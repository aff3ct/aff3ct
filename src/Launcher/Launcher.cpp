#include <stdexcept>
#include <cstdlib>
#include <regex>
#include <date.h>
#ifdef AFF3CT_MPI
#include <mpi.h>
#endif

#include "Tools/Display/Terminal/Terminal.hpp"
#include "Tools/general_utils.h"
#include "Tools/system_functions.h"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Factory/Header.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"
#include "Factory/Module/Puncturer/Puncturer.hpp"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Decoder/Decoder.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"

#include "Launcher/Launcher.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

Launcher::Launcher(const int argc, const char **argv, factory::Simulation &params_common,
                   std::ostream &stream)
: simu(nullptr), ah(argc, argv), params_common(params_common), stream(stream)
{
	cmd_line += std::string(argv[0]) + std::string(" ");
	for (auto i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			cmd_line += std::string(argv[i]);
		else
			cmd_line += std::string("\"") + std::string(argv[i]) + std::string("\"");

		cmd_line += std::string(" ");
	}
}

void Launcher::get_description_args()
{
}

void Launcher::store_args()
{
}

int Launcher::read_arguments()
{
	this->get_description_args();

	std::vector<std::string> cmd_error;

	this->arg_vals = ah.parse_arguments(this->args, this->cmd_warn, cmd_error);

	try
	{
		this->store_args();
		ah.set_help_display_keys(params_common.display_keys);
	}
	catch(const std::exception& e)
	{
		auto save = tools::exception::no_stacktrace;
		tools::exception::no_stacktrace = true;
		cmd_error.emplace_back(e.what());
		tools::exception::no_stacktrace = save;
	}

#ifdef AFF3CT_MPI
	if (this->params_common.mpi_rank == 0)
	{
#endif
		if (params_common.display_help)
		{
			auto grps = factory::Factory::create_groups({&params_common});
			ah.print_help(this->args, grps, params_common.display_adv_help);
		}

		// print usage
		if (!cmd_error.empty() && !params_common.display_help)
			ah.print_usage(this->args);

		// print the errors
		if (!cmd_error.empty()) std::cerr << std::endl;
		for (unsigned e = 0; e < cmd_error.size(); e++)
			std::cerr << rang::tag::error << cmd_error[e] << std::endl;

		// print the help tags
		if (!cmd_error.empty() && !params_common.display_help)
		{
			cli::Argument_tag help_tag = {"help", "h"};

			std::string message = "For more information please display the help (\"";
			message += cli::Argument_handler::print_tag(help_tag) += "\").";

			std::cerr << std::endl << rang::tag::info << message << std::endl;
		}
#ifdef AFF3CT_MPI
	}
#endif

	return (!cmd_error.empty() || params_common.display_help) ? EXIT_FAILURE : EXIT_SUCCESS;
}

void Launcher::print_header()
{
	// display configuration and simulation parameters
	stream << rang::tag::comment << rang::style::bold << "----------------------------------------------------" << std::endl;
	stream << rang::tag::comment << rang::style::bold << "---- A FAST FORWARD ERROR CORRECTION TOOLBOX >> ----" << std::endl;
	stream << rang::tag::comment << rang::style::bold << "----------------------------------------------------" << std::endl;
	stream << rang::tag::comment << rang::style::bold << rang::style::underline << "Parameters:"<< rang::style::reset << std::endl;
	tools::Header::print_parameters({&params_common}, this->params_common.full_legend, this->stream);
	this->stream << rang::tag::comment << std::endl;
}

std::string remove_argument(const std::string& cmd, std::string arg)
{
#if !defined(__clang__) && !defined(__llvm__) && defined(__GNUC__) && defined(__cplusplus) && __GNUC__ < 5
	if (arg.front() != ' ')
		arg = " " + arg;
	auto pos_arg = cmd.find(arg);

	if (pos_arg == std::string::npos)
		return cmd;

	auto pos_start = cmd.find("\"", pos_arg + arg.size());
	auto pos_end   = cmd.find("\"", pos_start + 1);

	return cmd.substr(0, pos_arg) + cmd.substr(pos_end + 1);
#else
	return std::regex_replace(cmd, std::regex("( " + arg + " \"[^\"]*\")"), "");
#endif
}

std::string remove_argument(std::string cmd, const std::vector<std::string>& args)
{
	for (auto& a : args)
		cmd = remove_argument(cmd, a);
	return cmd;
}

int Launcher::launch()
{
	int exit_code = EXIT_SUCCESS;

	std::srand((unsigned)this->params_common.global_seed);

	if (this->read_arguments() == EXIT_FAILURE)
	{
		// print the warnings
#ifdef AFF3CT_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			for (unsigned w = 0; w < this->cmd_warn.size(); w++)
				std::clog << rang::tag::warning << this->cmd_warn[w] << std::endl;
		return EXIT_FAILURE;
	}

	// write the command and he curve name in the PyBER format
#ifdef AFF3CT_MPI
	if (this->params_common.mpi_rank == 0)
#endif
	if (!this->params_common.meta.empty())
	{
		stream << "[metadata]" << std::endl;
		stream << "command=" << remove_argument(cmd_line, {"--sim-meta", "-t", "--ter-freq"}) << std::endl;
		stream << "title=" << this->params_common.meta << std::endl;
		stream << std::endl << "[trace]" << std::endl;
	}

	if (this->params_common.display_legend)
#ifdef AFF3CT_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			this->print_header();

	// print the warnings
#ifdef AFF3CT_MPI
	if (this->params_common.mpi_rank == 0)
#endif
		for (unsigned w = 0; w < this->cmd_warn.size(); w++)
			std::clog << rang::tag::warning << this->cmd_warn[w] << std::endl;

	try
	{
		simu.reset(this->build_simu());
	}
	catch(const std::exception& e)
	{
		rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
		exit_code = EXIT_FAILURE;
	}

	if (simu != nullptr)
	{
		// launch the simulation
		if (this->params_common.display_legend)
#ifdef AFF3CT_MPI
			if (this->params_common.mpi_rank == 0)
#endif
				stream << rang::tag::comment << "The simulation is running..." << std::endl;

		try
		{
			simu->launch();
			if (simu->is_error())
				exit_code = EXIT_FAILURE;
		}
		catch(const std::exception& e)
		{
			rang::format_on_each_line(std::cerr, std::string(e.what()) + "\n", rang::tag::error);
			exit_code = EXIT_FAILURE;
		}
	}

	if (this->params_common.display_legend)
#ifdef AFF3CT_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			stream << rang::tag::comment << "End of the simulation." << std::endl;

	return exit_code;
}
