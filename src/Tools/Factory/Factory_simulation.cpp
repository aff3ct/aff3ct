#include <cmath>
#include <chrono>

#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory_simulation.hpp"

using namespace aff3ct::tools;

void Factory_simulation::build_args(tools::Arguments_reader::arg_map &req_args, tools::Arguments_reader::arg_map &opt_args)
{
	// ---------------------------------------------------------------------------------------------------- simulation
	req_args[{"sim-snr-min", "m"}] =
		{"float",
		 "minimal signal/noise ratio to simulate."};

	req_args[{"sim-snr-max", "M"}] =
		{"float",
		 "maximal signal/noise ratio to simulate."};

	opt_args[{"sim-snr-step", "s"}] =
		{"positive_float",
		 "signal/noise ratio step between each simulation."};

	opt_args[{"sim-type"}] =
		{"string",
		 "select the type of simulation to launch (default is BFER).",
		 "BFER, BFERI, EXIT, GEN"};

	opt_args[{"sim-pyber"}] =
		{"string",
		 "prepare the output for the PyBER plotter tool, takes the name of the curve in PyBER."};

	opt_args[{"sim-stop-time"}] =
		{"positive_int",
		 "time in sec after what the current SNR iteration should stop."};

#ifndef STARPU
	opt_args[{"sim-threads", "t"}] =
		{"positive_int",
		 "enable multi-threaded mode and specify the number of threads."};
#else
	opt_args[{"sim-conc-tasks", "t"}] =
		{"positive_int",
		 "set the task concurrency level (default is 1, no concurrency)."};
#endif

#ifdef MULTI_PREC
	opt_args[{"sim-prec", "p"}] =
		{"positive_int",
		 "the simulation precision in bit.",
		 "8, 16, 32, 64"};
#endif

	opt_args[{"sim-inter-lvl"}] =
		{"positive_int",
		 "set the number of inter frame level to process in each modules."};

	opt_args[{"sim-seed"}] =
		{"positive_int",
		 "seed used in the simulation to initialize the pseudo random generators in general."};

#ifdef ENABLE_MPI
	opt_args[{"sim-mpi-comm"}] =
		{"positive_int",
		 "MPI communication frequency between the nodes (in millisec)."};
#endif

#ifdef ENABLE_COOL_BASH
	opt_args[{"sim-no-colors"}] =
		{"",
		 "disable the colors in the shell."};
#endif


	// ---------------------------------------------------------------------------------------------------------- code
	req_args[{"cde-type"}] =
		{"string",
		 "select the code type you want to use.",
		 "POLAR, TURBO, LDPC, REPETITION, RA, RSC, BCH, UNCODED" };

	req_args[{"cde-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (only information bits)."};

	req_args[{"cde-size", "N"}] =
		{"positive_int",
		 "total number of bit transmitted (includes parity bits)."};


	// ------------------------------------------------------------------------------------------------------- decoder
	opt_args[{"dec-type", "D"}] =
		{"string",
		 "select the algorithm you want to decode the codeword."};

	opt_args[{"dec-implem"}] =
		{"string",
		 "select the implementation of the algorithm to decode."};


	// ------------------------------------------------------------------------------------------------------ terminal
	opt_args[{"term-no"}] =
		{"",
		 "disable reporting for each iteration."};

	opt_args[{"term-freq"}] =
		{"positive_int",
		 "display frequency in ms (refresh time step for each iteration, 0 = disable display refresh)."};


	// --------------------------------------------------------------------------------------------------------- other
	opt_args[{"help", "h"}] =
		{"",
		 "print this help."};

	opt_args[{"version", "v"}] =
		{"",
		 "print informations about the version of the code."};

}

void Factory_simulation::store_args(const tools::Arguments_reader& ar, tools::parameters &params)
{
	using namespace std::chrono;

	// -------------------------------------------------------------------------------------------- default parameters
	params.simulation .snr_step          = 0.1f;
	params.simulation .n_threads         = 1;
	params.simulation .stop_time         = std::chrono::seconds(0);
	params.simulation .inter_frame_level = 1;
	params.simulation .mpi_rank          = 0;
	params.simulation .mpi_size          = 1;
	params.simulation .mpi_comm_freq     = std::chrono::milliseconds(1000);
	params.simulation .pyber             = "";
	params.simulation .snr_type          = "EB";
	params.simulation .seed              = 0;
	params.simulation .display_help      = false;
	params.simulation .display_version   = false;
	params.interleaver.seed              = 0;
	params.interleaver.uniform           = false;
	params.code       .tail_length       = 0;
	params.crc        .poly              = "";
	params.crc        .size              = 0;
	params.crc        .inc_code_rate     = false;

	// ---------------------------------------------------------------------------------------------------- simulation
	params.simulation.snr_min = ar.get_arg_float({"sim-snr-min", "m"}); // required
	params.simulation.snr_max = ar.get_arg_float({"sim-snr-max", "M"}); // required

	params.simulation.snr_max += 0.0001f; // hack to avoid the miss of the last snr

	if(ar.exist_arg({"sim-type"         })) params.simulation.type              = ar.get_arg      ({"sim-type"         });
	if(ar.exist_arg({"sim-pyber"        })) params.simulation.pyber             = ar.get_arg      ({"sim-pyber"        });
	if(ar.exist_arg({"sim-snr-step", "s"})) params.simulation.snr_step          = ar.get_arg_float({"sim-snr-step", "s"});
	if(ar.exist_arg({"sim-inter-lvl"    })) params.simulation.inter_frame_level = ar.get_arg_int  ({"sim-inter-lvl"    });
	if(ar.exist_arg({"sim-stop-time"    })) params.simulation.stop_time = seconds(ar.get_arg_int  ({"sim-stop-time"    }));
	if(ar.exist_arg({"sim-seed"         })) params.simulation.seed              = ar.get_arg_int  ({"sim-seed"         });

	params.interleaver.seed = params.simulation.seed;

#ifndef STARPU
	if(ar.exist_arg({"sim-threads", "t"}) && ar.get_arg_int({"sim-threads", "t"}) > 0)
		if(ar.exist_arg({"sim-threads", "t"})) params.simulation.n_threads = ar.get_arg_int  ({"sim-threads",    "t"});
#else
	if(ar.exist_arg({"sim-conc-tasks", "t"})) params.simulation.n_threads  = ar.get_arg_int  ({"sim-conc-tasks", "t"});
#endif

#ifdef ENABLE_MPI
	if(ar.exist_arg({"sim-mpi-comm"       })) params.simulation.mpi_comm_freq = milliseconds(ar.get_arg_int({"sim-mpi-comm"}));

	int max_n_threads_global;
	int max_n_threads_local = params.simulation.n_threads;

	MPI_Allreduce(&max_n_threads_local, &max_n_threads_global, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	if (max_n_threads_global <= 0)
	{
		std::stringstream message;
		message << "'max_n_threads_global' has to be greater than 0 ('max_n_threads_global' = "
		        << max_n_threads_global << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ensure that all the MPI processes have a different seed (crucial for the Monte-Carlo method)
	params.simulation.seed = max_n_threads_global * params.simulation.mpi_rank + params.simulation.seed;
#endif

#ifdef ENABLE_COOL_BASH
	// disable the cool bash mode for PyBER
	if (!params.simulation.pyber.empty())
		enable_bash_tools = false;

	if (ar.exist_arg({"sim-no-colors"})) enable_bash_tools = false;
#endif


	// ---------------------------------------------------------------------------------------------------------- code
	params.code.type   = ar.get_arg    ({"cde-type"          }); // required
	params.code.K      = ar.get_arg_int({"cde-info-bits", "K"}); // required
	params.code.K_info = ar.get_arg_int({"cde-info-bits", "K"});
	params.code.N      = ar.get_arg_int({"cde-size",      "N"}); // required
	params.code.N_code = ar.get_arg_int({"cde-size",      "N"});
	params.code.m      = (int)std::ceil(std::log2(params.code.N));

	params.code.R = params.code.K / (float)params.code.N;

	// ------------------------------------------------------------------------------------------------------- decoder
	if(ar.exist_arg({"dec-type",  "D"})) params.decoder.type   = ar.get_arg({"dec-type",  "D"});
	if(ar.exist_arg({"dec-implem"    })) params.decoder.implem = ar.get_arg({"dec-implem"    });


	// ------------------------------------------------------------------------------------------------------ terminal
	if(ar.exist_arg({"term-no"  })) params.terminal.disabled = true;
	if(ar.exist_arg({"term-no"  })) std::cerr<< "term-no found" << std::endl;
	if(ar.exist_arg({"term-freq"})) params.terminal.frequency = milliseconds(ar.get_arg_int({"term-freq"}));


	// --------------------------------------------------------------------------------------------------------- other
	if(ar.exist_arg({"help",    "h"})) params.simulation.display_help    = true;
	if(ar.exist_arg({"version", "v"})) params.simulation.display_version = true;
}

void Factory_simulation::group_args(tools::Arguments_reader::arg_grp& ar)
{
	ar.push_back({"sim",  "Simulation parameter(s)" });
	ar.push_back({"cde",  "Code parameter(s)"       });
	ar.push_back({"dec",  "Decoder parameter(s)"    });
	ar.push_back({"term", "Terminal parameter(s)"   });
}
