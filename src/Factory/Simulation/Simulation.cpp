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
::get_description(tools::Argument_map_info &req_args, tools::Argument_map_info &opt_args) const
{
	Launcher::parameters::get_description(req_args, opt_args);

	auto p = this->get_prefix();

	req_args.add(
		{p+"-snr-min", "m"},
		new tools::Real<>(),
		"minimal signal/noise ratio to simulate.");

	req_args.add(
		{p+"-snr-max", "M"},
		new tools::Real<>(),
		"maximal signal/noise ratio to simulate.");

	opt_args.add(
		{p+"-snr-step", "s"},
		new tools::Real<>({new tools::Positive<float>(), new tools::Non_zero<float>()}),
		"signal/noise ratio step between each simulation.");

	opt_args.add(
		{p+"-pyber"},
		new tools::Text<>(),
		"prepare the output for the PyBER plotter tool, takes the name of the curve in PyBER.");

	opt_args.add(
		{p+"-stop-time"},
		new tools::Integer<>({new tools::Positive<int>()}),
		"time in sec after what the current SNR iteration should stop (0 is infinite).");

	opt_args.add(
		{p+"-debug"},
		new tools::None(),
		"enable debug mode: print array values after each step.");

	opt_args.add(
		{p+"-debug-prec"},
		new tools::Integer<>({new tools::Min<int>(2)}),
		"set the precision of real elements when displayed in debug mode.");

	opt_args.add(
		{p+"-debug-limit", "d"},
		new tools::Integer<>({new tools::Positive<int>(), new tools::Non_zero<int>()}),
		"set the max number of elements to display in the debug mode.");

	opt_args.add(
		{p+"-stats"},
		new tools::None(),
		"display statistics module by module.");

	opt_args.add(
		{p+"-threads", "t"},
		new tools::Integer<>({new tools::Positive<int>()}),
		"enable multi-threaded mode and specify the number of threads (0 means the maximum supported by the core.");

	opt_args.add(
		{p+"-seed", "S"},
		new tools::Integer<>({new tools::Positive<int>()}),
		"seed used in the simulation to initialize the pseudo random generators in general.");

#ifdef ENABLE_MPI
	opt_args.add(
		{p+"-mpi-comm"},
		new tools::Integer<>({new tools::Positive<int>(), new tools::Non_zero<int>()}),
		"MPI communication frequency between the nodes (in millisec).");
#endif

#ifdef ENABLE_COOL_BASH
	opt_args.add(
		{p+"-no-colors"},
		new tools::None(),
		"disable the colors in the shell.");
#endif
}

void Simulation::parameters
::store(const tools::Argument_map_value &vals)
{
	using namespace std::chrono;

	Launcher::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-snr-min",  "m"   })) this->snr_min     =         vals.to_float({p+"-snr-min",  "m"});
	if(vals.exist({p+"-snr-max",  "M"   })) this->snr_max     =         vals.to_float({p+"-snr-max",  "M"});
	if(vals.exist({p+"-snr-step", "s"   })) this->snr_step    =         vals.to_float({p+"-snr-step", "s"});
	if(vals.exist({p+"-pyber"           })) this->pyber       =         vals.at      ({p+"-pyber"        });
	if(vals.exist({p+"-stop-time"       })) this->stop_time   = seconds(vals.to_int  ({p+"-stop-time"    }));
	if(vals.exist({p+"-seed",     "S"   })) this->global_seed =         vals.to_int  ({p+"-seed",     "S"});
	if(vals.exist({p+"-stats"           })) this->statistics  = true;
	if(vals.exist({p+"-debug"           })) this->debug       = true;
	if(vals.exist({p+"-debug-limit", "d"}))
	{
		this->debug = true;
		this->debug_limit = vals.to_int({p+"-debug-limit", "d"});
	}
	if(vals.exist({p+"-debug-prec"}))
	{
		this->debug = true;
		this->debug_precision = vals.to_int({p+"-debug-prec"});
	}

	this->snr_max += 0.0001f; // hack to avoid the miss of the last snr

	if(vals.exist({p+"-threads", "t"}) && vals.to_int({p+"-threads", "t"}) > 0)
		if(vals.exist({p+"-threads", "t"})) this->n_threads = vals.to_int({p+"-threads", "t"});

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &this->mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->mpi_rank);

	if(exist(vals, {p+"-mpi-comm"})) this->mpi_comm_freq = milliseconds(vals.to_int({p+"-mpi-comm"}));

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

	if (vals.exist({p+"-no-colors"})) tools::enable_bash_tools = false;
#endif

#ifdef MULTI_PREC
	if(vals.exist({p+"-prec", "p"})) this->sim_prec = vals.to_int({p+"-prec", "p"});
#endif

	if (this->debug && !(vals.exist({p+"-threads", "t"}) && vals.to_int({p+"-threads", "t"}) > 0))
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
