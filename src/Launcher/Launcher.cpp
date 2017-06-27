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

#include "Tools/Factory/Factory_simulation.hpp"
#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_modem.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"

#include "Launcher.hpp"

using namespace aff3ct::tools;
using namespace aff3ct::simulation;
using namespace aff3ct::launcher;

template <typename B, typename R, typename Q>
Launcher<B,R,Q>
::Launcher(const int argc, const char **argv, std::ostream &stream)
: max_n_chars(0), simu(nullptr), ar(argc, argv), stream(stream)
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

	// define type names
	type_names[typeid(char)]        = "char ("        + std::to_string(sizeof(char)*8)        + " bits)";
	type_names[typeid(signed char)] = "signed char (" + std::to_string(sizeof(signed char)*8) + " bits)";
	type_names[typeid(short)]       = "short ("       + std::to_string(sizeof(short)*8)       + " bits)";
	type_names[typeid(int)]         = "int ("         + std::to_string(sizeof(int)*8)         + " bits)";
	type_names[typeid(long long)]   = "long long ("   + std::to_string(sizeof(long long)*8)   + " bits)";
	type_names[typeid(float)]       = "float ("       + std::to_string(sizeof(float)*8)       + " bits)";
	type_names[typeid(double)]      = "double ("      + std::to_string(sizeof(double)*8)      + " bits)";

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &params.simulation.mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &params.simulation.mpi_rank);
#endif
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
	Factory_simulation    ::build_args(req_args, opt_args);

	Factory_source<B>     ::build_args(req_args, opt_args);

	Factory_modem<B,R,Q>  ::build_args(req_args, opt_args);

	Factory_channel<R>    ::build_args(req_args, opt_args);

	Factory_quantizer<R,Q>::build_args(req_args, opt_args);
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::store_args()
{
	Factory_simulation    ::store_args(ar, params);

	Factory_source<B>     ::store_args(ar, params);

	Factory_modem<B,R,Q>  ::store_args(ar, params);

	Factory_channel<R>    ::store_args(ar, params);

	Factory_quantizer<R,Q>::store_args(ar, params);
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::group_args()
{
	Factory_simulation    ::group_args(arg_group);

	Factory_source<B>     ::group_args(arg_group);

	Factory_modem<B,R,Q>  ::group_args(arg_group);

	Factory_channel<R>    ::group_args(arg_group);

	Factory_quantizer<R,Q>::group_args(arg_group);
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

	if (params.simulation.display_help)
	{
		this->group_args();

		arg_group.push_back({"itl",  "Interleaver parameter(s)"});
		arg_group.push_back({"pct",  "Puncturer parameter(s)"  });
		arg_group.push_back({"dpct", "Depuncturer parameter(s)"});
		arg_group.push_back({"enc",  "Encoder parameter(s)"    });
		arg_group.push_back({"mnt",  "Monitor parameter(s)"    });

		ar.print_usage(arg_group);
		error = true; // in order to exit at the end of this function
	}

	// print the errors
	for (unsigned e = 0; e < cmd_error.size(); e++)
		std::cerr << format_error(cmd_error[e]) << std::endl;

	if(miss_arg)
		std::cerr << format_error("A required argument is missing.") << std::endl;

	// print the warnings
	if (params.simulation.mpi_rank == 0)
		for (unsigned w = 0; w < cmd_warn.size(); w++)
			std::clog << format_warning(cmd_warn[w]) << std::endl;

	// print the help tags
	if ((miss_arg || error) && !params.simulation.display_help)
	{
		std::string message = "For more information please display the help (";
		std::vector<std::string> help_tag = {"help", "h"};
		for (unsigned i = 0; i < help_tag.size(); i++)
			message += Arguments_reader::print_tag(help_tag[i]) + ((i < help_tag.size()-1)?", ":"");

		message += ").";
		std::cerr << format_info(message) << std::endl;
	}

	std::exit(1);

	return (error?EXIT_FAILURE:EXIT_SUCCESS);
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_simulation()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Type",          params.simulation.type));
	p.push_back(std::make_pair("SNR min (m)",   std::to_string(params.simulation.snr_min)  + " dB"));
	p.push_back(std::make_pair("SNR max (M)",   std::to_string(params.simulation.snr_max)  + " dB"));
	p.push_back(std::make_pair("SNR step (s)",  std::to_string(params.simulation.snr_step) + " dB"));
	p.push_back(std::make_pair("Type of bits",  type_names[typeid(B)]));
	p.push_back(std::make_pair("Type of reals", type_names[typeid(R)]));
	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
		p.push_back(std::make_pair("Type of quant. reals", type_names[typeid(Q)]));
	p.push_back(std::make_pair("Inter frame level", std::to_string(params.simulation.inter_frame_level)));
	p.push_back(std::make_pair("Seed", std::to_string(params.simulation.seed)));

#ifdef ENABLE_MPI
	p.push_back(std::make_pair("MPI comm. freq. (ms)", std::to_string(params.simulation.mpi_comm_freq.count())));
	p.push_back(std::make_pair("MPI size", std::to_string(params.simulation.mpi_size)));
#endif

	using namespace date;
	using namespace std::chrono;

	std::stringstream date;
	date << system_clock::now();

	auto split_date = string_split(date.str(), '.');
	p.push_back(std::make_pair("Date (UTC)", split_date[0]));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_code()
{
	std::vector<std::pair<std::string,std::string>> p;

	std::string N = std::to_string(params.code.N - params.code.tail_length);
	if (params.code.tail_length > 0)
		N += " + " + std::to_string(params.code.tail_length) + " (tail bits)";

	std::stringstream K;
	if (!params.crc.poly.empty())
		K << (params.code.K - params.crc.size) << " + " << params.crc.size << " (CRC)";
	else
		K << params.code.K;

	p.push_back(std::make_pair("Type",              params.code.type             ));
	p.push_back(std::make_pair("Info. bits (K)",    K.str()                      ));
	p.push_back(std::make_pair("Codeword size (N)", N                            ));
	p.push_back(std::make_pair("Code rate (R)",     std::to_string(params.code.R)));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_source()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Type", params.source.type));

	if (params.source.type == "USER")
		p.push_back(std::make_pair("Path", params.source.path));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_crc()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_encoder()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_puncturer()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_interleaver()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_modulator()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Type", this->params.modulator.type));
	if (params.modulator.type == "CPM")
	{
		if(params.modulator.cpm_std.size())
			p.push_back(std::make_pair("CPM standard", this->params.modulator.cpm_std));

		p.push_back(std::make_pair("CPM L memory", std::to_string(this->params.modulator.cpm_L)    ));
		p.push_back(std::make_pair("CPM h index", (std::to_string(this->params.modulator.cpm_k) +
		                                           std::string("/")                             +
		                                           std::to_string(this->params.modulator.cpm_p))   ));
		p.push_back(std::make_pair("CPM wave shape",              this->params.modulator.wave_shape));
		p.push_back(std::make_pair("CPM mapping",                 this->params.modulator.mapping   ));
	}

	p.push_back(std::make_pair("Bits per symbol", std::to_string(this->params.modulator.bits_per_symbol)));
	p.push_back(std::make_pair("Sampling factor", std::to_string(this->params.modulator.upsample_factor)));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_channel()
{
	std::vector<std::pair<std::string,std::string>> p;

	p.push_back(std::make_pair("Type", params.channel.type));

	if (params.channel.type == "USER")
		p.push_back(std::make_pair("Path", params.channel.path));

	if (params.channel.type.find("RAYLEIGH") != std::string::npos)
		p.push_back(std::make_pair("Block fading policy", params.channel.block_fading));

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_demodulator()
{
	std::vector<std::pair<std::string,std::string>> p;

	std::string demod_sig2 = (params.demodulator.no_sig2) ? "off" : "on";
	std::string demod_max  = (params.modulator.type == "BPSK"     ) ||
	                         (params.modulator.type == "BPSK_FAST") ||
	                         (params.modulator.type == "SCMA"     ) ?
	                         "unused" : params.demodulator.max;
	std::string demod_ite  = std::to_string(params.demodulator.n_ite);
	std::string demod_psi  = params.demodulator.psi;

	p.push_back(std::make_pair("Sigma square", demod_sig2));
	p.push_back(std::make_pair("Max type",     demod_max ));
	if (params.modulator.type == "SCMA")
	{
		p.push_back(std::make_pair("Number of iterations", demod_ite));
		p.push_back(std::make_pair("Psi function",         demod_psi));
	}

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_depuncturer()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_quantizer()
{
	std::vector<std::pair<std::string,std::string>> p;

	if ((typeid(Q) != typeid(float)) && (typeid(Q) != typeid(double)))
	{
		std::string quantif = "unused";
		if (type_names[typeid(R)] != type_names[typeid(Q)])
		{
			if (params.quantizer.type == "TRICKY")
				quantif = "{"+std::to_string(params.quantizer.n_bits)+", "+std::to_string(params.quantizer.range)+"f}";
			else
				quantif = "{"+std::to_string(params.quantizer.n_bits)+", "+std::to_string(params.quantizer.n_decimals)+"}";
		}

		p.push_back(std::make_pair("Type"               , params.quantizer.type));
		p.push_back(std::make_pair("Fixed-point config.", quantif              ));
	}

	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_decoder()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_monitor()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
std::vector<std::pair<std::string,std::string>> Launcher<B,R,Q>
::header_terminal()
{
	std::vector<std::pair<std::string,std::string>> p;
	return p;
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::compute_max_n_chars()
{
	std::function<void (std::vector<std::pair<std::string,std::string>>)> compute_max =
		[&](std::vector<std::pair<std::string,std::string>> params)
	{
		for (auto i = 0; i < (int)params.size(); i++)
			this->max_n_chars = std::max(this->max_n_chars, (int)params[i].first.length());
	};

	compute_max(header_simulation ());
	compute_max(header_code       ());
	compute_max(header_source     ());
	compute_max(header_crc        ());
	compute_max(header_encoder    ());
	compute_max(header_puncturer  ());
	compute_max(header_interleaver());
	compute_max(header_modulator  ());
	compute_max(header_channel    ());
	compute_max(header_demodulator());
	compute_max(header_depuncturer());
	compute_max(header_quantizer  ());
	compute_max(header_decoder    ());
	compute_max(header_monitor    ());
	compute_max(header_terminal   ());
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::print_parameters(std::string grp_name, std::vector<std::pair<std::string,std::string>> params)
{
	stream << "# * " << style(style(grp_name, Style::BOLD), Style::UNDERLINED) << " ";
	for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
	stream << std::endl;

	for (auto i = 0; i < (int)params.size(); i++)
	{
		stream << "#    ** " << style(params[i].first, Style::BOLD);
		for (auto j = 0; j < this->max_n_chars - (int)params[i].first.length(); j++) stream << " ";
		stream << " = " << params[i].second << std::endl;
	}
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::print_header()
{
	this->compute_max_n_chars();

	// display configuration and simulation parameters
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style("---- A FAST FORWARD ERROR CORRECTION TOOL >> ----", Style::BOLD) << std::endl;
	stream << "# " << style("-------------------------------------------------", Style::BOLD) << std::endl;
	stream << "# " << style(style("Parameters :", Style::BOLD), Style::UNDERLINED) << std::endl;

	std::vector<std::pair<std::string,std::string>> params;
	params = this->header_simulation();  if (params.size()) this->print_parameters("Simulation",  params);
	params = this->header_code();        if (params.size()) this->print_parameters("Code",        params);
	params = this->header_source();      if (params.size()) this->print_parameters("Source",      params);
	params = this->header_crc();         if (params.size()) this->print_parameters("CRC",         params);
	params = this->header_encoder();     if (params.size()) this->print_parameters("Encoder",     params);
	params = this->header_puncturer();   if (params.size()) this->print_parameters("Puncturer",   params);
	params = this->header_interleaver(); if (params.size()) this->print_parameters("Interleaver", params);
	params = this->header_modulator();   if (params.size()) this->print_parameters("Modulator",   params);
	params = this->header_channel();     if (params.size()) this->print_parameters("Channel",     params);
	params = this->header_demodulator(); if (params.size()) this->print_parameters("Demodulator", params);
	params = this->header_depuncturer(); if (params.size()) this->print_parameters("Depuncturer", params);
	params = this->header_quantizer();   if (params.size()) this->print_parameters("Quantizer",   params);
	params = this->header_decoder();     if (params.size()) this->print_parameters("Decoder",     params);
	params = this->header_monitor();     if (params.size()) this->print_parameters("Monitor",     params);
	params = this->header_terminal();    if (params.size()) this->print_parameters("Terminal",    params);
	stream << "#" << std::endl;
}

template <typename B, typename R, typename Q>
void Launcher<B,R,Q>
::launch()
{
	std::srand(params.simulation.seed);

	// in case of the user call launch multiple times
	if (simu != nullptr)
	{
		delete simu;
		simu = nullptr;
	}

	if (this->read_arguments() == EXIT_FAILURE)
		return;

	// write the command and he curve name in the PyBER format
	if (!params.simulation.pyber.empty() && params.simulation.mpi_rank == 0)
	{
		stream << "Run command:"          << std::endl;
		stream << cmd_line                << std::endl;
		stream << "Curve name:"           << std::endl;
		stream << params.simulation.pyber << std::endl;
	}

	if (params.simulation.mpi_rank == 0)
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
