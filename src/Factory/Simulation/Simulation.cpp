#include "Tools/Exception/exception.hpp"

#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Simulation::name   = "Simulation";
const std::string aff3ct::factory::Simulation::prefix = "sim";

Simulation::parameters
::parameters(const std::string name, const std::string prefix)
: Launcher::parameters(name, Simulation::name, prefix)
{
}

Simulation::parameters* Simulation::parameters
::clone() const
{
	return new Simulation::parameters(*this);
}

Simulation::parameters
::~parameters()
{
}

void Simulation::parameters
::get_description(arg_map &req_args, arg_map &opt_args) const
{
	Launcher::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args[{p+"-snr-min", "m"}] =
		{"float",
		 "minimal signal/noise ratio to simulate."};

	req_args[{p+"-snr-max", "M"}] =
		{"float",
		 "maximal signal/noise ratio to simulate."};

	opt_args[{p+"-snr-step", "s"}] =
		{"strictly_positive_float",
		 "signal/noise ratio step between each simulation."};

	opt_args[{p+"-pyber"}] =
		{"string",
		 "prepare the output for the PyBER plotter tool, takes the name of the curve in PyBER."};

	opt_args[{p+"-stop-time"}] =
		{"positive_int",
		 "time in sec after what the current SNR iteration should stop."};

	opt_args[{p+"-debug", "d"}] =
		{"",
		 "enable debug mode: print array values after each step."};

	opt_args[{p+"-debug-prec"}] =
		{"positive_int",
		 "set the precision of real elements when displayed in debug mode."};

	opt_args[{p+"-debug-limit"}] =
		{"positive_int",
		 "set the max number of elements to display in the debug mode."};

	opt_args[{p+"-stats"}] =
		{"",
		 "display statistics module by module."};

	opt_args[{p+"-threads", "t"}] =
		{"positive_int",
		 "enable multi-threaded mode and specify the number of threads."};

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

void Simulation::parameters
::store(const arg_val_map &vals)
{
	using namespace std::chrono;

	Launcher::parameters::store(vals);

	auto p = this->get_prefix();

	if(exist(vals, {p+"-snr-min",  "m"})) this->snr_min     =         std::stof(vals.at({p+"-snr-min",  "m"}));
	if(exist(vals, {p+"-snr-max",  "M"})) this->snr_max     =         std::stof(vals.at({p+"-snr-max",  "M"}));
	if(exist(vals, {p+"-pyber"        })) this->pyber       =                   vals.at({p+"-pyber"        });
	if(exist(vals, {p+"-snr-step", "s"})) this->snr_step    =         std::stof(vals.at({p+"-snr-step", "s"}));
	if(exist(vals, {p+"-stop-time"    })) this->stop_time   = seconds(std::stoi(vals.at({p+"-stop-time"    })));
	if(exist(vals, {p+"-seed",     "S"})) this->global_seed =         std::stoi(vals.at({p+"-seed",     "S"}));
	if(exist(vals, {p+"-stats"        })) this->statistics  = true;
	if(exist(vals, {p+"-debug",    "d"})) this->debug       = true;
	if(exist(vals, {p+"-debug-limit"}))
	{
		this->debug = true;
		this->debug_limit = std::stoi(vals.at({p+"-debug-limit"}));
	}
	if(exist(vals, {p+"-debug-prec"}))
	{
		this->debug = true;
		this->debug_precision = std::stoi(vals.at({p+"-debug-prec"}));
	}

	this->snr_max += 0.0001f; // hack to avoid the miss of the last snr

	if(exist(vals, {p+"-threads", "t"}) && std::stoi(vals.at({p+"-threads", "t"})) > 0)
		if(exist(vals, {p+"-threads", "t"})) this->n_threads = std::stoi(vals.at({p+"-threads", "t"}));

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &this->mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->mpi_rank);

	if(exist(vals, {p+"-mpi-comm"})) this->mpi_comm_freq = milliseconds(std::stoi(vals.at({p+"-mpi-comm"})));

	int max_n_threads_global;
	int max_n_threads_local = this->n_threads;

	MPI_Allreduce(&max_n_threads_local, &max_n_threads_global, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	if (max_n_threads_global <= 0)
	{
		std::stringstream message;
		message << "'max_n_threads_global' has to be greater than 0 ('max_n_threads_global' = "
		        << max_n_threads_global << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// ensure that all the MPI processes have a different seed (crucial for the Monte-Carlo method)
	this->local_seed = this->global_seed + max_n_threads_global * this->mpi_rank;
#else
	this->local_seed = this->global_seed;
#endif

#ifdef ENABLE_COOL_BASH
	// disable the cool bash mode for PyBER
	if (!this->pyber.empty())
		tools::enable_bash_tools = false;

	if (exist(vals, {p+"-no-colors"})) tools::enable_bash_tools = false;
#endif

#ifdef MULTI_PREC
	if(exist(vals, {p+"-prec", "p"})) this->sim_prec = std::stoi(vals.at({p+"-prec", "p"}));
#endif

	if (this->debug && !(exist(vals, {p+"-threads", "t"}) && std::stoi(vals.at({p+"-threads", "t"})) > 0))
		// check if debug is asked and if n_thread kept its default value
		this->n_threads = 1;
}

void Simulation::parameters
::get_headers(std::map<std::string,header_list>& headers, const bool full) const
{
	Launcher::parameters::get_headers(headers);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("SNR min (m)",  std::to_string(this->snr_min)  + " dB"));
	headers[p].push_back(std::make_pair("SNR max (M)",  std::to_string(this->snr_max)  + " dB"));
	headers[p].push_back(std::make_pair("SNR step (s)", std::to_string(this->snr_step) + " dB"));
	headers[p].push_back(std::make_pair("Seed", std::to_string(this->global_seed)));
	headers[p].push_back(std::make_pair("Statistics", this->statistics ? "on" : "off"));
	headers[p].push_back(std::make_pair("Debug mode", this->debug ? "on" : "off"));
	if (this->debug)
	{
		headers[p].push_back(std::make_pair("Debug precision", std::to_string(this->debug_precision)));
		if (this->debug_limit)
			headers[p].push_back(std::make_pair("Debug limit", std::to_string(this->debug_limit)));
	}

#ifdef ENABLE_MPI
	headers[p].push_back(std::make_pair("MPI comm. freq. (ms)", std::to_string(this->mpi_comm_freq.count())));
	headers[p].push_back(std::make_pair("MPI size",             std::to_string(this->mpi_size             )));
#endif

	std::string threads = "unused";
	if (this->n_threads)
		threads = std::to_string(this->n_threads) + " thread(s)";

	headers[p].push_back(std::make_pair("Multi-threading (t)", threads));
}
