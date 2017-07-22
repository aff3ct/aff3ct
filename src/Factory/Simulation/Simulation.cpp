#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Simulation::name   = "Simulation";
const std::string aff3ct::factory::Simulation::prefix = "sim";

void Simulation::build_args(arg_map &req_args, arg_map &opt_args, const std::string p)
{
	Launcher::build_args(req_args, opt_args, p);

	req_args[{p+"-snr-min", "m"}] =
		{"float",
		 "minimal signal/noise ratio to simulate."};

	req_args[{p+"-snr-max", "M"}] =
		{"float",
		 "maximal signal/noise ratio to simulate."};

	opt_args[{p+"-snr-step", "s"}] =
		{"positive_float",
		 "signal/noise ratio step between each simulation."};

	opt_args[{p+"-pyber"}] =
		{"string",
		 "prepare the output for the PyBER plotter tool, takes the name of the curve in PyBER."};

	opt_args[{p+"-stop-time"}] =
		{"positive_int",
		 "time in sec after what the current SNR iteration should stop."};

#ifndef STARPU
	opt_args[{p+"-threads", "t"}] =
		{"positive_int",
		 "enable multi-threaded mode and specify the number of threads."};
#else
	opt_args[{p+"-conc-tasks", "t"}] =
		{"positive_int",
		 "set the task concurrency level (default is 1, no concurrency)."};
#endif

	opt_args[{p+"-seed", "S"}] =
		{"positive_int",
		 "seed used in the simulation to initialize the pseudo random generators in general."};

#ifdef ENABLE_MPI
	opt_args[{p+"-mpi-comm"}] =
		{"positive_int",
		 "MPI communication frequency between the nodes (in millisec)."};
#endif

#ifdef ENABLE_COOL_BASH
	opt_args[{p+"-no-colors"}] =
		{"",
		 "disable the colors in the shell."};
#endif
}

void Simulation::store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p)
{
	using namespace std::chrono;

	Launcher::store_args(ar, params, p);

	if(ar.exist_arg({p+"-snr-min",  "m"})) params.snr_min =           ar.get_arg_float({p+"-snr-min",  "m"});
	if(ar.exist_arg({p+"-snr-max",  "M"})) params.snr_max =           ar.get_arg_float({p+"-snr-max",  "M"});
	if(ar.exist_arg({p+"-pyber"        })) params.pyber     =         ar.get_arg      ({p+"-pyber"        });
	if(ar.exist_arg({p+"-snr-step", "s"})) params.snr_step  =         ar.get_arg_float({p+"-snr-step", "s"});
	if(ar.exist_arg({p+"-stop-time"    })) params.stop_time = seconds(ar.get_arg_int  ({p+"-stop-time"    }));
	if(ar.exist_arg({p+"-seed",     "S"})) params.seed      =         ar.get_arg_int  ({p+"-seed",     "S"});

	params.snr_max += 0.0001f; // hack to avoid the miss of the last snr

#ifndef STARPU
	if(ar.exist_arg({p+"-threads", "t"}) && ar.get_arg_int({p+"-threads", "t"}) > 0)
		if(ar.exist_arg({p+"-threads", "t"})) params.n_threads = ar.get_arg_int({p+"-threads",    "t"});
#else
	if(ar.exist_arg({p+"-conc-tasks", "t"})) params.n_threads  = ar.get_arg_int({p+"-conc-tasks", "t"});
#endif

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &params.mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &params.mpi_rank);

	if(ar.exist_arg({p+"-mpi-comm"})) params.mpi_comm_freq = milliseconds(ar.get_arg_int({p+"-mpi-comm"}));

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
		tools::enable_bash_tools = false;

	if (ar.exist_arg({p+"-no-colors"})) tools::enable_bash_tools = false;
#endif

#ifdef MULTI_PREC
	if(ar.exist_arg({p+"-prec", "p"})) params.sim_prec = ar.get_arg_int({p+"-prec", "p"});
#endif
}

void Simulation::header(params_list& head_sim, const parameters& params)
{
	Launcher::header(head_sim, params);

	head_sim.push_back(std::make_pair("SNR min (m)",   std::to_string(params.snr_min)  + " dB"));
	head_sim.push_back(std::make_pair("SNR max (M)",   std::to_string(params.snr_max)  + " dB"));
	head_sim.push_back(std::make_pair("SNR step (s)",  std::to_string(params.snr_step) + " dB"));

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
}
