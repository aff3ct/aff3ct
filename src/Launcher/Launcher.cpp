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

#include "Launcher.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
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

	std::vector<std::string> cmd_warn, cmd_error;

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
		std::cerr << format_error(cmd_error[e]) << std::endl;

	if(miss_arg)
		std::cerr << format_error("At least one required argument is missing.") << std::endl;

	// print the warnings
#ifdef ENABLE_MPI
	if (simu_params->mpi_rank == 0)
#endif
		for (unsigned w = 0; w < cmd_warn.size(); w++)
			std::clog << format_warning(cmd_warn[w]) << std::endl;

	// print the help tags
	if ((miss_arg || error) && !params->display_help)
	{
		std::string message = "For more information please display the help (";
		std::vector<std::string> help_tag = {"help", "h"};
		for (unsigned i = 0; i < help_tag.size(); i++)
			message += Arguments_reader::print_tag(help_tag[i]) + ((i < help_tag.size()-1)?", ":"");

		message += ").";
		std::cerr << format_info(message) << std::endl;
	}

	return ((miss_arg || error)?EXIT_FAILURE:EXIT_SUCCESS);
}

void Launcher::print_header()
{
	// display configuration and simulation parameters
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----", Style::BOLD) << std::endl;
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style(style("Parameters :", Style::BOLD), Style::UNDERLINED) << std::endl;

	int max_n_chars = 0;
	Factory::Header::compute_max_n_chars(pl_sim,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_src,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_crc,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_enc,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_pct,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_itl,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_mod,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_chn,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_demod, max_n_chars);
	Factory::Header::compute_max_n_chars(pl_qnt,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_dec,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_mnt,   max_n_chars);
	Factory::Header::compute_max_n_chars(pl_ter,   max_n_chars);

	if (pl_sim  .size()) Factory::Header::print_parameters("Simulation",  pl_sim,   max_n_chars, this->stream);
	if (pl_src  .size()) Factory::Header::print_parameters("Source",      pl_src,   max_n_chars, this->stream);
	if (pl_crc  .size()) Factory::Header::print_parameters("CRC",         pl_crc,   max_n_chars, this->stream);
	if (pl_enc  .size()) Factory::Header::print_parameters("Encoder",     pl_enc,   max_n_chars, this->stream);
	if (pl_pct  .size()) Factory::Header::print_parameters("Puncturer",   pl_pct,   max_n_chars, this->stream);
	if (pl_itl  .size()) Factory::Header::print_parameters("Interleaver", pl_itl,   max_n_chars, this->stream);
	if (pl_mod  .size()) Factory::Header::print_parameters("Modulator",   pl_mod,   max_n_chars, this->stream);
	if (pl_chn  .size()) Factory::Header::print_parameters("Channel",     pl_chn,   max_n_chars, this->stream);
	if (pl_demod.size()) Factory::Header::print_parameters("Demodulator", pl_demod, max_n_chars, this->stream);
	if (pl_qnt  .size()) Factory::Header::print_parameters("Quantizer",   pl_qnt,   max_n_chars, this->stream);
	if (pl_dec  .size()) Factory::Header::print_parameters("Decoder",     pl_dec,   max_n_chars, this->stream);
	if (pl_mnt  .size()) Factory::Header::print_parameters("Monitor",     pl_mnt,   max_n_chars, this->stream);
	if (pl_ter  .size()) Factory::Header::print_parameters("Terminal",    pl_ter,   max_n_chars, this->stream);
	this->stream << "#" << std::endl;
}

void Launcher::launch()
{
	std::srand(this->params->seed);

	// in case of the user call launch multiple times
	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}

	if (this->read_arguments() == EXIT_FAILURE)
		return;

	// write the command and he curve name in the PyBER format
#ifdef ENABLE_MPI
	if (!params_sim->pyber.empty() && params_sim->mpi_rank == 0)
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
	if (simu_params->mpi_rank == 0)
#endif
		this->print_header();

	try
	{
		simu = this->build_simu();
	}
	catch (std::exception const& e)
	{
		std::cerr << apply_on_each_line(e.what(), &format_error) << std::endl;
	}

	if (simu != nullptr)
	{
		// launch the simulation
#ifdef ENABLE_MPI
	if (simu_params->mpi_rank == 0)
#endif
			stream << "# " << "The simulation is running..." << std::endl;

		try
		{
			simu->launch();
		}
		catch (std::exception const& e)
		{
			std::cerr << apply_on_each_line(e.what(), &format_error) << std::endl;
		}
	}

#ifdef ENABLE_MPI
	if (simu_params->mpi_rank == 0)
#endif
		stream << "# End of the simulation." << std::endl;

	if (simu != nullptr)
		delete simu;
}
