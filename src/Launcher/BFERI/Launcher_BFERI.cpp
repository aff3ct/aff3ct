#include <string>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Launcher_BFERI.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI<B,R,Q>
::Launcher_BFERI(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream)
{
	this->params.simulation .type           = "BFERI";
	this->params.simulation .benchs         = 0;
	this->params.simulation .benchs_no_ldst = false;
	this->params.simulation .debug          = false;
	this->params.simulation .debug_limit    = 0;
	this->params.simulation .time_report    = false;
	this->params.simulation .trace_path     = "";
	this->params.encoder    .systematic     = true;
	this->params.interleaver.type           = "RANDOM";
	this->params.demodulator.max            = "MAX";
	this->params.demodulator.n_ite          = 30;
	this->params.monitor    .n_frame_errors = 100;
	this->params.terminal   .type           = "STD";
}

template <typename B, typename R, typename Q>
void Launcher_BFERI<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	this->opt_args[{"sim-benchs", "b"}] =
		{"positive_int",
		 "enable special benchmark mode with a loop around the decoder."};
	this->opt_args[{"sim-benchs-no-ldst"}] =
		{"",
		 "enable the display of the decoder throughput considering only the decoder time."};
	this->opt_args[{"sim-debug", "d"}] =
		{"",
		 "enable debug mode: print array values after each step."};
	this->opt_args[{"sim-debug-limit"}] =
		{"positive_int",
		 "set the max number of elements to display in the debug mode."};
	this->opt_args[{"sim-trace-path"}] =
		{"string",
		 "traces array values in a CSV file."};
	this->opt_args[{"sim-time-report"}] =
		{"",
		 "display time information about the simulation chain."};

	// --------------------------------------------------------------------------------------------------- interleaver
	this->opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, RANDOM_HARD, GOLDEN, NO"};

	// --------------------------------------------------------------------------------------------------- demodulator
	this->opt_args[{"dmod-ite"}] =
		{"positive_int",
		 "number of iterations in the turbo demodulation."};

	// ------------------------------------------------------------------------------------------------------- monitor
	this->opt_args[{"mnt-max-fe", "e"}] =
		{"positive_int",
		 "max number of frame errors for each SNR simulation."};

	// ------------------------------------------------------------------------------------------------------ terminal
	this->opt_args[{"term-type"}] =
		{"string",
		 "select the terminal you want.",
		 "STD, LEGACY"};
}

template <typename B, typename R, typename Q>
void Launcher_BFERI<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	// ---------------------------------------------------------------------------------------------------- simulation
	if(this->ar.exist_arg({"sim-trace-path"    })) this->params.simulation.trace_path   = this->ar.get_arg    ({"sim-trace-path" });
	if(this->ar.exist_arg({"sim-benchs",    "b"})) this->params.simulation.benchs       = this->ar.get_arg_int({"sim-benchs", "b"});
	if(this->ar.exist_arg({"sim-time-report"   })) this->params.simulation.time_report  = true;
	if(this->ar.exist_arg({"sim-debug",     "d"})) this->params.simulation.debug        = true;
	if(this->ar.exist_arg({"sim-debug-limit"   }))
	{
		this->params.simulation.debug = true;
		this->params.simulation.debug_limit  = this->ar.get_arg_int({"sim-debug-limit"});
	}

	// --------------------------------------------------------------------------------------------------- interleaver
	if(this->ar.exist_arg({"itl-type"})) this->params.interleaver.type = this->ar.get_arg({"itl-type"});

	// --------------------------------------------------------------------------------------------------- demodulator
	if(this->ar.exist_arg({"dmod-ite"})) this-> params.demodulator.n_ite = this->ar.get_arg_int({"dmod-ite"});

	// ------------------------------------------------------------------------------------------------------- monitor
	if(this->ar.exist_arg({"mnt-max-fe", "e"})) this->params.monitor.n_frame_errors = this->ar.get_arg_int({"mnt-max-fe", "e"});

	// ------------------------------------------------------------------------------------------------------ terminal
	if(this->ar.exist_arg({"term-type"})) this->params.terminal.type = this->ar.get_arg({"term-type"});
}

template <typename B, typename R, typename Q>
void Launcher_BFERI<B,R,Q>
::print_header()
{
	Launcher<B,R,Q>::print_header();

	std::string syst_enc = ((this->params.encoder.systematic) ? "on" : "off");

	std::string threads = "unused";
	if (this->params.simulation.n_threads)
		threads = std::to_string(this->params.simulation.n_threads) + " thread(s)";

	// display configuration and simulation parameters
	this->stream << "# " << bold("* Max frame error count     (FE)") << " = " << this->params.monitor.n_frame_errors << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->params.interleaver.type       << std::endl;
	this->stream << "# " << bold("* Number of turbo demod. ite.   ") << " = " << this->params.demodulator.n_ite      << std::endl;
	this->stream << "# " << bold("* Systematic encoding           ") << " = " << syst_enc                            << std::endl;
	this->stream << "# " << bold("* Decoding algorithm            ") << " = " << this->params.decoder.type           << std::endl;
	this->stream << "# " << bold("* Decoding implementation       ") << " = " << this->params.decoder.implem         << std::endl;
	this->stream << "# " << bold("* Multi-threading               ") << " = " << threads                             << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Launcher_BFERI<B_8,R_8,Q_8>;
template class Launcher_BFERI<B_16,R_16,Q_16>;
template class Launcher_BFERI<B_32,R_32,Q_32>;
template class Launcher_BFERI<B_64,R_64,Q_64>;
#else
template class Launcher_BFERI<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
