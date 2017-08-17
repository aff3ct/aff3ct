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
#include "Tools/Display/bash_tools.h"
#include "Tools/Exception/exception.hpp"

#include "Factory/Module/Source.hpp"
#include "Factory/Module/CRC.hpp"
#include "Factory/Module/Code/Encoder.hpp"
#include "Factory/Module/Code/Puncturer.hpp"
#include "Factory/Module/Interleaver.hpp"
#include "Factory/Module/Modem.hpp"
#include "Factory/Module/Channel.hpp"
#include "Factory/Module/Quantizer.hpp"
#include "Factory/Module/Code/Decoder.hpp"
#include "Factory/Module/Monitor.hpp"
#include "Factory/Tools/Display/Terminal/Terminal.hpp"

#include "Launcher.hpp"

using namespace aff3ct;
using namespace aff3ct::launcher;

Launcher::Launcher(const int argc, const char **argv, std::ostream &stream)
: simu(nullptr), ar(argc, argv), params(nullptr), stream(stream)
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
	if (params != nullptr) { delete params; params = nullptr; };
}

void Launcher::build_args()
{
}

void Launcher::store_args()
{
}

void Launcher::group_args()
{
}

int Launcher::read_arguments()
{
	this->build_args();

	std::vector<std::string> cmd_error;

	bool miss_arg = !ar.parse_arguments(req_args, opt_args, cmd_warn);
	bool error    = !ar.check_arguments(cmd_error);

	this->store_args();

	if (params->display_help)
	{
		this->group_args();

		ar.print_usage(arg_group);
		error = true; // in order to exit at the end of this function
	}

	// print the errors
	for (unsigned e = 0; e < cmd_error.size(); e++)
		std::cerr << tools::format_error(cmd_error[e]) << std::endl;

	if (miss_arg)
		std::cerr << tools::format_error("At least one required argument is missing.") << std::endl;

	// print the help tags
	if ((miss_arg || error) && !params->display_help)
	{
		std::string message = "For more information please display the help (";
		std::vector<std::string> help_tag = {"help", "h"};
		for (unsigned i = 0; i < help_tag.size(); i++)
			message += tools::Arguments_reader::print_tag(help_tag[i]) + ((i < help_tag.size()-1)?", ":"");

		message += ").";
		std::cerr << tools::format_info(message) << std::endl;
	}

	return ((miss_arg || error) ? EXIT_FAILURE : EXIT_SUCCESS);
}

void Launcher::print_header()
{
	// display configuration and simulation parameters
	stream << "# " << tools::style("-------------------------------------------------", tools::Style::BOLD) << std::endl;
	stream << "# " << tools::style("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----", tools::Style::BOLD) << std::endl;
	stream << "# " << tools::style("-------------------------------------------------", tools::Style::BOLD) << std::endl;
	stream << "# " << tools::style(style("Parameters :", tools::Style::BOLD), tools::Style::UNDERLINED) << std::endl;

	int max_n_chars = 0;
	factory::Header::compute_max_n_chars(pl_sim, max_n_chars);
	factory::Header::compute_max_n_chars(pl_cde, max_n_chars);
	factory::Header::compute_max_n_chars(pl_src, max_n_chars);
	factory::Header::compute_max_n_chars(pl_crc, max_n_chars);
	factory::Header::compute_max_n_chars(pl_enc, max_n_chars);
	factory::Header::compute_max_n_chars(pl_pct, max_n_chars);
	factory::Header::compute_max_n_chars(pl_itl, max_n_chars);
	factory::Header::compute_max_n_chars(pl_mdm, max_n_chars);
	factory::Header::compute_max_n_chars(pl_chn, max_n_chars);
	factory::Header::compute_max_n_chars(pl_qnt, max_n_chars);
	factory::Header::compute_max_n_chars(pl_dec, max_n_chars);
	factory::Header::compute_max_n_chars(pl_mnt, max_n_chars);
	factory::Header::compute_max_n_chars(pl_ter, max_n_chars);

	if (pl_sim.size()) factory::Header::print_parameters(factory::Simulation ::name, pl_sim, max_n_chars, this->stream);
	if (pl_cde.size()) factory::Header::print_parameters("Code"                    , pl_cde, max_n_chars, this->stream);
	if (pl_src.size()) factory::Header::print_parameters(factory::Source     ::name, pl_src, max_n_chars, this->stream);
	if (pl_crc.size()) factory::Header::print_parameters(factory::CRC        ::name, pl_crc, max_n_chars, this->stream);
	if (pl_enc.size()) factory::Header::print_parameters(factory::Encoder    ::name, pl_enc, max_n_chars, this->stream);
	if (pl_pct.size()) factory::Header::print_parameters(factory::Puncturer  ::name, pl_pct, max_n_chars, this->stream);
	if (pl_itl.size()) factory::Header::print_parameters(factory::Interleaver::name, pl_itl, max_n_chars, this->stream);
	if (pl_mdm.size()) factory::Header::print_parameters(factory::Modem      ::name, pl_mdm, max_n_chars, this->stream);
	if (pl_chn.size()) factory::Header::print_parameters(factory::Channel    ::name, pl_chn, max_n_chars, this->stream);
	if (pl_qnt.size()) factory::Header::print_parameters(factory::Quantizer  ::name, pl_qnt, max_n_chars, this->stream);
	if (pl_dec.size()) factory::Header::print_parameters(factory::Decoder    ::name, pl_dec, max_n_chars, this->stream);
	if (pl_mnt.size()) factory::Header::print_parameters(factory::Monitor    ::name, pl_mnt, max_n_chars, this->stream);
	if (pl_ter.size()) factory::Header::print_parameters(factory::Terminal   ::name, pl_ter, max_n_chars, this->stream);
	this->stream << "#" << std::endl;
}

void Launcher::launch()
{
	std::srand(this->params->global_seed);

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
		if (this->params->mpi_rank == 0)
#endif
			for (unsigned w = 0; w < cmd_warn.size(); w++)
				std::clog << tools::format_warning(cmd_warn[w]) << std::endl;
		return;
	}

	// write the command and he curve name in the PyBER format
#ifdef ENABLE_MPI
	if (!this->params->pyber.empty() && this->params->mpi_rank == 0)
#else
	if (!this->params->pyber.empty())
#endif
	{
		stream << "Run command:"      << std::endl;
		stream << cmd_line            << std::endl;
		stream << "Curve name:"       << std::endl;
		stream << this->params->pyber << std::endl;
	}

#ifdef ENABLE_MPI
	if (this->params->mpi_rank == 0)
#endif
		this->print_header();

	// print the warnings
#ifdef ENABLE_MPI
	if (this->params->mpi_rank == 0)
#endif
		for (unsigned w = 0; w < cmd_warn.size(); w++)
			std::clog << tools::format_warning(cmd_warn[w]) << std::endl;

	try
	{
		simu = this->build_simu();
	}
	catch (std::exception const& e)
	{
		std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
	}

	if (simu != nullptr)
	{
		// launch the simulation
#ifdef ENABLE_MPI
	if (this->params->mpi_rank == 0)
#endif
			stream << "# " << "The simulation is running..." << std::endl;

		try
		{
			simu->launch();
		}
		catch (std::exception const& e)
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
		}
	}

#ifdef ENABLE_MPI
	if (this->params->mpi_rank == 0)
#endif
		stream << "# End of the simulation." << std::endl;

	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}
}
