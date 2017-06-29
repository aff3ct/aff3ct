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
#include "Tools/Factory/Factory_modem.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Exception/exception.hpp"


#include "Launcher.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher<B,R,Q>
::Launcher(const int argc, const char **argv, std::ostream &stream)
: simu(nullptr), ar(argc, argv), stream(stream)
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

template <typename B, typename R, typename Q>
Launcher<B,R,Q>
::~Launcher() 
{
	if (simu != nullptr) delete simu;
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::build_args()
{
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::store_args()
{
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::group_args()
{
}

template <typename B, typename R, typename Q>
int Launcher<B,R,Q>
::read_arguments()
{
	this->build_args();

	std::vector<std::string> cmd_warn, cmd_error;

	bool miss_arg = !ar.parse_arguments(req_args, opt_args, cmd_warn);
	bool error    = !ar.check_arguments(cmd_error);

	this->store_args();

	if (simu_params->display_help)
	{
		this->group_args();

		arg_group.push_back({"itl",  "Interleaver parameter(s)"});
		arg_group.push_back({"pct",  "Puncturer parameter(s)"  });
		arg_group.push_back({"dpct", "Depuncturer parameter(s)"});

		ar.print_usage(arg_group);
		error = true; // in order to exit at the end of this function
	}

	// print the errors
	for (unsigned e = 0; e < cmd_error.size(); e++)
		std::cerr << format_error(cmd_error[e]) << std::endl;

	if(miss_arg)
		std::cerr << format_error("A required argument is missing.") << std::endl;

	// print the warnings
#ifdef ENABLE_MPI
	if (simu_params->mpi_rank == 0)
#endif
		for (unsigned w = 0; w < cmd_warn.size(); w++)
			std::clog << format_warning(cmd_warn[w]) << std::endl;

	// print the help tags
	if ((miss_arg || error) && !simu_params->display_help)
	{
		std::string message = "For more information please display the help (";
		std::vector<std::string> help_tag = {"help", "h"};
		for (unsigned i = 0; i < help_tag.size(); i++)
			message += Arguments_reader::print_tag(help_tag[i]) + ((i < help_tag.size()-1)?", ":"");

		message += ").";
		std::cerr << format_info(message) << std::endl;
	}

	return (error?EXIT_FAILURE:EXIT_SUCCESS);
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::compute_max_n_chars()
{
//	std::function<void (Header::params_list)> compute_max =
//		[&](Header::params_list params)
//	{
//		for (auto i = 0; i < (int)params.size(); i++)
//			this->max_n_chars = std::max(this->max_n_chars, (int)params[i].first.length());
//	};

//	compute_max(header_simulation ());
//	compute_max(header_code       ());
//	compute_max(header_source     ());
//	compute_max(header_crc        ());
//	compute_max(header_encoder    ());
//	compute_max(header_puncturer  ());
//	compute_max(header_interleaver());
//	compute_max(header_modulator  ());
//	compute_max(header_channel    ());
//	compute_max(header_demodulator());
//	compute_max(header_depuncturer());
//	compute_max(header_quantizer  ());
//	compute_max(header_decoder    ());
//	compute_max(header_monitor    ());
//	compute_max(header_terminal   ());
}


template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::print_header()
{
//	this->compute_max_n_chars();

	// display configuration and simulation parameters
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----", Style::BOLD) << std::endl;
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style(style("Parameters :", Style::BOLD), Style::UNDERLINED) << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::launch()
{
	std::srand(simu_params->seed);

	// in case of the user call launch multiple times
	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}

	if (this->read_arguments() == EXIT_FAILURE)
		return;

	// write the command and he curve name in the PyBER format
	if (!simu_params->pyber.empty()
#ifdef ENABLE_MPI
			&& simu_params->mpi_rank == 0
#endif
	   )
	{
		stream << "Run command:"          << std::endl;
		stream << cmd_line                << std::endl;
		stream << "Curve name:"           << std::endl;
		stream << simu_params->pyber << std::endl;
	}

#ifdef ENABLE_MPI
	if (simu_params->mpi_rank == 0)
#endif
		this->print_header();

	std::exit(1);

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
		if (params.simulation.mpi_rank == 0)
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

	if (params.simulation.mpi_rank == 0)
		stream << "# End of the simulation." << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::launcher::Launcher<B_8, R_8, Q_8 >;
template class aff3ct::launcher::Launcher<B_16,R_16,Q_16>;
template class aff3ct::launcher::Launcher<B_32,R_32,Q_32>;
template class aff3ct::launcher::Launcher<B_64,R_64,Q_64>;
#else
template class aff3ct::launcher::Launcher<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
