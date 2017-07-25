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

void Simulation::store_args(const arg_val_map &vals, parameters &params, const std::string p)
{
	using namespace std::chrono;

	Launcher::store_args(vals, params, p);

	if(exist(vals, {p+"-snr-min",  "m"})) params.snr_min     =         std::stof(vals.at({p+"-snr-min",  "m"}));
	if(exist(vals, {p+"-snr-max",  "M"})) params.snr_max     =         std::stof(vals.at({p+"-snr-max",  "M"}));
	if(exist(vals, {p+"-pyber"        })) params.pyber       =                   vals.at({p+"-pyber"        });
	if(exist(vals, {p+"-snr-step", "s"})) params.snr_step    =         std::stof(vals.at({p+"-snr-step", "s"}));
	if(exist(vals, {p+"-stop-time"    })) params.stop_time   = seconds(std::stoi(vals.at({p+"-stop-time"    })));
	if(exist(vals, {p+"-seed",     "S"})) params.global_seed =         std::stoi(vals.at({p+"-seed",     "S"}));

	params.snr_max += 0.0001f; // hack to avoid the miss of the last snr

#ifndef STARPU
	if(exist(vals, {p+"-threads", "t"}) && std::stoi(vals.at({p+"-threads", "t"})) > 0)
		if(exist(vals, {p+"-threads", "t"})) params.n_threads = std::stoi(vals.at({p+"-threads",    "t"}));
#else
	if(exist(vals, {p+"-conc-tasks", "t"})) params.n_threads  = std::stoi(vals.at({p+"-conc-tasks", "t"}));
#endif

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &params.mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &params.mpi_rank);

	if(exist(vals, {p+"-mpi-comm"})) params.mpi_comm_freq = milliseconds(std::stoi(vals.at({p+"-mpi-comm"})));

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
	params.local_seed = params.global_seed + max_n_threads_global * params.mpi_rank;
#endif

#ifdef ENABLE_COOL_BASH
	// disable the cool bash mode for PyBER
	if (!params.pyber.empty())
		tools::enable_bash_tools = false;

	if (exist(vals, {p+"-no-colors"})) tools::enable_bash_tools = false;
#endif

#ifdef MULTI_PREC
	if(exist(vals, {p+"-prec", "p"})) params.sim_prec = std::stoi(vals.at({p+"-prec", "p"}));
#endif
}

void Simulation::make_header(params_list& head_sim, const parameters& params)
{
	Launcher::make_header(head_sim, params);

	head_sim.push_back(std::make_pair("SNR min (m)",   std::to_string(params.snr_min)  + " dB"));
	head_sim.push_back(std::make_pair("SNR max (M)",   std::to_string(params.snr_max)  + " dB"));
	head_sim.push_back(std::make_pair("SNR step (s)",  std::to_string(params.snr_step) + " dB"));

	head_sim.push_back(std::make_pair("Seed", std::to_string(params.global_seed)));

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
