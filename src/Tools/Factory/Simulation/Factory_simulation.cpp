#include "Factory_simulation.hpp"

using namespace aff3ct;
using namespace tools;

void Factory_simulation::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	Factory_simulation_main::build_args(req_args, opt_args);

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
	req_args[{"cde-info-bits", "K"}] =
		{"positive_int",
		 "useful number of bit transmitted (only information bits)."};

	req_args[{"cde-size", "N"}] =
		{"positive_int",
		 "total number of bit transmitted (includes parity bits)."};
}

void Factory_simulation::store_args(const Arguments_reader& ar, parameters &params)
{
	using namespace std::chrono;

	Factory_simulation_main::store_args(ar, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	params.snr_min = ar.get_arg_float({"sim-snr-min", "m"}); // required
	params.snr_max = ar.get_arg_float({"sim-snr-max", "M"}); // required

	params.snr_max += 0.0001f; // hack to avoid the miss of the last snr

	if(ar.exist_arg({"sim-pyber"        })) params.pyber             = ar.get_arg      ({"sim-pyber"        });
	if(ar.exist_arg({"sim-snr-step", "s"})) params.snr_step          = ar.get_arg_float({"sim-snr-step", "s"});
	if(ar.exist_arg({"sim-inter-lvl"    })) params.inter_frame_level = ar.get_arg_int  ({"sim-inter-lvl"    });
	if(ar.exist_arg({"sim-stop-time"    })) params.stop_time = seconds(ar.get_arg_int  ({"sim-stop-time"    }));
	if(ar.exist_arg({"sim-seed"         })) params.seed              = ar.get_arg_int  ({"sim-seed"         });

#ifndef STARPU
	if(ar.exist_arg({"sim-threads", "t"}) && ar.get_arg_int({"sim-threads", "t"}) > 0)
		if(ar.exist_arg({"sim-threads", "t"})) params.n_threads = ar.get_arg_int({"sim-threads",    "t"});
#else
	if(ar.exist_arg({"sim-conc-tasks", "t"})) params.n_threads  = ar.get_arg_int({"sim-conc-tasks", "t"});
#endif

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &params.mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &params.mpi_rank);

	if(ar.exist_arg({"sim-mpi-comm"})) params.mpi_comm_freq = milliseconds(ar.get_arg_int({"sim-mpi-comm"}));

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
	params.seed += max_n_threads_global * params.mpi_rank;
#endif

#ifdef ENABLE_COOL_BASH
	// disable the cool bash mode for PyBER
	if (!params.pyber.empty())
		enable_bash_tools = false;

	if (ar.exist_arg({"sim-no-colors"})) enable_bash_tools = false;
#endif

#ifdef MULTI_PREC
	if(ar.exist_arg({"sim-prec", "p"})) params.sim_prec = ar.get_arg_int({"sim-prec", "p"});
#endif

	// ---------------------------------------------------------------------------------------------------------- code
	params.K      = ar.get_arg_int({"cde-info-bits", "K"}); // required
	params.N      = ar.get_arg_int({"cde-size",      "N"}); // required

	params.K_info = params.K;
	params.N_code = params.N;
	params.R      = params.K / (float)params.N;
}

void Factory_simulation::group_args(Arguments_reader::arg_grp& ar)
{
	Factory_simulation_main::group_args(ar);
}

void Factory_simulation::header(Header::params_list& head_sim, Header::params_list& head_cde,
                                const parameters& params)
{
	Factory_simulation_main::header(head_sim, head_cde, params);

	// ---------------------------------------------------------------------------------------------------- simulation
	head_sim.push_back(std::make_pair("SNR min (m)",   std::to_string(params.snr_min)  + " dB"));
	head_sim.push_back(std::make_pair("SNR max (M)",   std::to_string(params.snr_max)  + " dB"));
	head_sim.push_back(std::make_pair("SNR step (s)",  std::to_string(params.snr_step) + " dB"));

	head_sim.push_back(std::make_pair("Inter frame level", std::to_string(params.inter_frame_level)));
	head_sim.push_back(std::make_pair("Seed", std::to_string(params.seed)));

#ifdef ENABLE_MPI
	head_sim.push_back(std::make_pair("MPI comm. freq. (ms)", std::to_string(params.mpi_comm_freq.count())));
	head_sim.push_back(std::make_pair("MPI size",             std::to_string(params.mpi_size             )));
#endif

#ifdef STARPU
	head_sim.push_back(std::make_pair("Task concurrency level (t)", std::to_string(params.n_threads)));
#else
	std::string threads = "unused";
	if (params.n_threads)
		threads = std::to_string(params.n_threads) + " thread(s)";

	head_sim.push_back(std::make_pair("Multi-threading (t)", threads));
#endif

	// ---------------------------------------------------------------------------------------------------------- code

//	std::string N = std::to_string(params.code.N - params.code.tail_length);
//	if (params.code.tail_length > 0)
//		N += " + " + std::to_string(params.code.tail_length) + " (tail bits)";

//	std::stringstream K;
//	if (!params.crc.poly.empty())
//		K << (params.K - params.crc.size) << " + " << params.crc.size << " (CRC)";
//	else
//		K << params.K;
	head_cde.push_back(std::make_pair("Info. bits (K)",    std::to_string(params.K_info)));
	head_cde.push_back(std::make_pair("Codeword size (N)", std::to_string(params.N_code)));
	head_cde.push_back(std::make_pair("Code rate (R)",     std::to_string(params.R)));
}
