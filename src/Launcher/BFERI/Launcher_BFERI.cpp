#include <string>
#include <iostream>

#include "Tools/Display/bash_tools.h"

#include "Launcher_BFERI.hpp"

template <typename B, typename R, typename Q>
Launcher_BFERI<B,R,Q>
::Launcher_BFERI(const int argc, const char **argv, std::ostream &stream)
: Launcher<B,R,Q>(argc, argv, stream)
{
	this->params.simulation.type            = "BFERI";
	this->params.simulation.max_fe          = 100;
	this->params.simulation.benchs          = 0;
	this->params.simulation.enable_debug    = false;
	this->params.simulation.debug_limit     = 0;
	this->params.simulation.enable_leg_term = false;
	this->params.simulation.enable_dec_thr  = false;
	this->params.simulation.time_report     = false;
	this->params.simulation.trace_path_file = "";
	this->params.encoder.systematic         = true;
	this->params.modulator.demod_max        = "MAX";
	this->params.modulator.demod_n_ite      = 30;
	this->params.code.interleaver           = "RANDOM";
}

template <typename B, typename R, typename Q>
void Launcher_BFERI<B,R,Q>
::build_args()
{
	Launcher<B,R,Q>::build_args();

	this->opt_args[{"mnt-max-fe", "e"}] =
		{"positive_int",
		 "max number of frame errors for each SNR simulation."};
	this->opt_args[{"sim-benchs", "b"}] =
		{"positive_int",
		 "enable special benchmark mode with a loop around the decoder."};
	this->opt_args[{"term-legagy"}] =
		{"",
		 "enable the legacy display (needed for retro-compatibility with PyBER)."};
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
	this->opt_args[{"dmod-ite"}] =
		{"positive_int",
		 "number of iterations in the turbo demodulation."};
	this->opt_args[{"itl-type"}] =
		{"string",
		 "specify the type of the interleaver.",
		 "LTE, CCSDS, RANDOM, RANDOM_HARD, GOLDEN, NO"};
}

template <typename B, typename R, typename Q>
void Launcher_BFERI<B,R,Q>
::store_args()
{
	Launcher<B,R,Q>::store_args();

	// facultative parameters
	if(this->ar.exist_arg({"mnt-max-fe", "e"})) this->params.simulation.max_fe          = this->ar.get_arg_int({"mnt-max-fe", "e"});
	if(this->ar.exist_arg({"sim-benchs", "b"})) this->params.simulation.benchs          = this->ar.get_arg_int({"sim-benchs", "b"});
	if(this->ar.exist_arg({"term-legacy"    })) this->params.simulation.enable_leg_term = true;
	if(this->ar.exist_arg({"sim-debug",  "d"})) this->params.simulation.enable_debug    = true;
	if(this->ar.exist_arg({"sim-debug-limit"}))
	{
		this->params.simulation.enable_debug = true;
		this->params.simulation.debug_limit  = this->ar.get_arg_int({"sim-debug-limit"});
	}
	if(this->ar.exist_arg({"sim-time-report"})) this->params.simulation.time_report     = true;
	if(this->ar.exist_arg({"sim-trace-path" })) this->params.simulation.trace_path_file = this->ar.get_arg    ({"sim-trace-path"});
	if(this->ar.exist_arg({"dmod-ite"       })) this-> params.modulator.demod_n_ite     = this->ar.get_arg_int({"dmod-ite"      });
	if(this->ar.exist_arg({"itl-type"       })) this->params.code.interleaver           = this->ar.get_arg    ({"itl-type"      });
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
	this->stream << "# " << bold("* Max frame error count     (FE)") << " = " << this->params.simulation.max_fe     << std::endl;
	this->stream << "# " << bold("* Interleaver                   ") << " = " << this->params.code.interleaver      << std::endl;
	this->stream << "# " << bold("* Number of turbo demod. ite.   ") << " = " << this->params.modulator.demod_n_ite << std::endl;
	this->stream << "# " << bold("* Systematic encoding           ") << " = " << syst_enc                           << std::endl;
	this->stream << "# " << bold("* Decoding algorithm            ") << " = " << this->params.decoder.algo          << std::endl;
	this->stream << "# " << bold("* Decoding implementation       ") << " = " << this->params.decoder.implem        << std::endl;
	this->stream << "# " << bold("* Multi-threading               ") << " = " << threads                            << std::endl;
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
