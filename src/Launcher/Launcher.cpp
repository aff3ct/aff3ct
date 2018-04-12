#include <rang.hpp>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "Tools/date.h"
#include "Tools/general_utils.h"
#include "Tools/system_functions.h"
#include "Tools/Display/bash_tools.h"
#include "Tools/Exception/exception.hpp"

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

#include "Launcher.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

Launcher::Launcher(const int argc, const char **argv, factory::Simulation::parameters &params_common,
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

Launcher::~Launcher()
{
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
	}
	catch(const std::exception& e)
	{
		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		cmd_error.push_back(e.what());
		tools::exception::no_backtrace = save;
	}

	if (params_common.display_help)
	{
		auto grps = factory::Factory::create_groups({&params_common});
		ah.print_help(this->args, grps, params_common.display_adv_help);
	}

	// print usage
	if (cmd_error.size() && !params_common.display_help)
		ah.print_usage(this->args);

	// print the errors
	if (cmd_error.size()) std::cerr << std::endl;
	for (unsigned e = 0; e < cmd_error.size(); e++)
		std::cerr << tools::format_error(cmd_error[e]) << std::endl;

	// print the help tags
	if (cmd_error.size() && !params_common.display_help)
	{
		tools::Argument_tag help_tag = {"help", "h"};

		std::string message = "For more information please display the help (\"";
		message += tools::Argument_handler::print_tag(help_tag) += "\").";

		std::cerr << std::endl << tools::format_info(message) << std::endl;
	}

	return (cmd_error.size() || params_common.display_help) ? EXIT_FAILURE : EXIT_SUCCESS;
}

void Launcher::print_header()
{
	// display configuration and simulation parameters
	stream << "# " << rang::style::bold << "----------------------------------------------------" << std::endl;
	stream << "# " <<                      "---- A FAST FORWARD ERROR CORRECTION TOOLBOX >> ----" << std::endl;
	stream << "# " <<                      "----------------------------------------------------" << std::endl;
	stream << "# " << rang::style::underline << "Parameters :"<< rang::style::reset << std::endl;
	factory::Header::print_parameters({&params_common}, false, this->stream);
	this->stream << "#" << std::endl;
}

int Launcher::launch()
{
	int exit_code = EXIT_SUCCESS;

	std::srand(this->params_common.global_seed);

	// in case of the user call launch multiple times
	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}

	if (this->read_arguments() == EXIT_FAILURE)
	{
		// print the warnings
#ifdef ENABLE_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			for (unsigned w = 0; w < this->cmd_warn.size(); w++)
				std::clog << tools::format_warning(this->cmd_warn[w]) << std::endl;
		return EXIT_FAILURE;
	}

	// write the command and he curve name in the PyBER format
#ifdef ENABLE_MPI
	if (!this->params_common.pyber.empty() && this->params_common.mpi_rank == 0)
#else
	if (!this->params_common.pyber.empty())
#endif
	{
		stream << "Run command:"     << std::endl;
		stream << cmd_line           << std::endl;
		stream << "Curve name:"      << std::endl;
		stream << this->params_common.pyber << std::endl;
	}

	if (this->params_common.display_legend)
#ifdef ENABLE_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			this->print_header();

	// print the warnings
#ifdef ENABLE_MPI
	if (this->params_common.mpi_rank == 0)
#endif
		for (unsigned w = 0; w < this->cmd_warn.size(); w++)
			std::clog << tools::format_warning(this->cmd_warn[w]) << std::endl;

	try
	{
		simu = this->build_simu();
	}
	catch(const std::exception& e)
	{
		std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
		exit_code = EXIT_FAILURE;
	}

	if (simu != nullptr)
	{
		// launch the simulation
		if (this->params_common.display_legend)
#ifdef ENABLE_MPI
			if (this->params_common.mpi_rank == 0)
#endif
				stream << "# " << "The simulation is running..." << std::endl;

		try
		{
			simu->launch();
			if (simu->is_error())
				exit_code = EXIT_FAILURE;
		}
		catch(const std::exception& e)
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
			exit_code = EXIT_FAILURE;
		}
	}

	if (this->params_common.display_legend)
#ifdef ENABLE_MPI
		if (this->params_common.mpi_rank == 0)
#endif
			stream << "# End of the simulation." << std::endl;

	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}

	return exit_code;
}
