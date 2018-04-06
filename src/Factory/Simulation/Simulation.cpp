#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Simulation_name   = "Simulation";
const std::string aff3ct::factory::Simulation_prefix = "sim";

Simulation::parameters
::parameters(const std::string &name, const std::string &prefix)
: Launcher::parameters(name, Simulation_name, prefix)
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

struct SNR_range_D1_splitter : tools::Splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "{([";
		const std::string queue     = "})]";
		const std::string separator = ",";

		return Splitter::split(val, head, queue, separator);
	}
};

struct SNR_range_D2_splitter : tools::Splitter
{
	static std::vector<std::string> split(const std::string& val)
	{
		const std::string head      = "";
		const std::string queue     = "";
		const std::string separator = ":";

		return Splitter::split(val, head, queue, separator);
	}
};

void Simulation::parameters
::get_description(tools::Argument_map_info &args) const
{
	Launcher::parameters::get_description(args);

	auto p = this->get_prefix();

	args.add(
		{p+"-snr-range", "R"},
		tools::List2D<float,SNR_range_D1_splitter,SNR_range_D2_splitter>(
		              tools::Real(),
		              std::make_tuple(tools::Length(1)),
		              std::make_tuple(tools::Length(1,3))),
		"signal/noise ratio range to run (Matlab style: \"0.5:2.5,2.55,2.6:0.05:3\" with a default step of 0.1).",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-snr-min", "m"},
		tools::Real(),
		"minimal signal/noise ratio to simulate.",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-snr-max", "M"},
		tools::Real(),
		"maximal signal/noise ratio to simulate.",
		tools::Argument_info::REQUIRED);

	args.add(
		{p+"-snr-step", "s"},
		tools::Real(tools::Positive(), tools::Non_zero()),
		"signal/noise ratio step between each simulation.");


	args.add_link({p+"-snr-range", "R"}, {p+"-snr-min",  "m"});
	args.add_link({p+"-snr-range", "R"}, {p+"-snr-max",  "M"});
	args.add_link({p+"-snr-range", "R"}, {p+"-snr-step", "s"});



	args.add(
		{p+"-pyber"},
		tools::Text(),
		"prepare the output for the PyBER plotter tool, takes the name of the curve in PyBER.");

	args.add(
		{p+"-stop-time"},
		tools::Integer(tools::Positive()),
		"time in sec after what the current SNR iteration should stop (0 is infinite).");

	args.add(
		{p+"-debug"},
		tools::None(),
		"enable debug mode: print array values after each step.");

	args.add(
		{p+"-debug-hex"},
		tools::None(),
		"debug mode prints values in the hexadecimal format.");

	args.add(
		{p+"-debug-prec"},
		tools::Integer(tools::Positive()),
		"set the precision of real elements when displayed in debug mode.");

	args.add(
		{p+"-debug-limit", "d"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"set the max number of elements to display in the debug mode.");

	args.add(
		{p+"-stats"},
		tools::None(),
		"display statistics module by module.");

	args.add(
		{p+"-threads", "t"},
		tools::Integer(tools::Positive()),
		"enable multi-threaded mode and specify the number of threads (0 means the maximum supported by the core.");

	args.add(
		{p+"-seed", "S"},
		tools::Integer(tools::Positive()),
		"seed used in the simulation to initialize the pseudo random generators in general.");

#ifdef ENABLE_MPI
	args.add(
		{p+"-mpi-comm"},
		tools::Integer(tools::Positive(), tools::Non_zero()),
		"MPI communication frequency between the nodes (in millisec).");
#endif
}

void Simulation::parameters
::store(const tools::Argument_map_value &vals)
{
	using namespace std::chrono;

	Launcher::parameters::store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-snr-range", "R"}))
		this->snr_range = tools::generate_range(vals.to_list<std::vector<float>>({p+"-snr-range", "R"}), 0.1f);
	else
	{
		float snr_min = 0.f, snr_max = 0.f, snr_step = 0.1f;
		if(vals.exist({p+"-snr-min",  "m"})) snr_min  = vals.to_float({p+"-snr-min",  "m"});
		if(vals.exist({p+"-snr-max",  "M"})) snr_max  = vals.to_float({p+"-snr-max",  "M"});
		if(vals.exist({p+"-snr-step", "s"})) snr_step = vals.to_float({p+"-snr-step", "s"});

		this->snr_range = tools::generate_range({{snr_min, snr_max}}, snr_step);
	}

	if(vals.exist({p+"-pyber"           })) this->pyber       =         vals.at      ({p+"-pyber"    });
	if(vals.exist({p+"-stop-time"       })) this->stop_time   = seconds(vals.to_int  ({p+"-stop-time"}));
	if(vals.exist({p+"-seed",        "S"})) this->global_seed =         vals.to_int  ({p+"-seed", "S"});
	if(vals.exist({p+"-stats"           })) this->statistics  = true;
	if(vals.exist({p+"-debug"           })) this->debug       = true;
	if(vals.exist({p+"-debug-limit", "d"}))
	{
		this->debug = true;
		this->debug_limit = vals.to_int({p+"-debug-limit", "d"});
	}
	if(vals.exist({p+"-debug-hex"}))
	{
		this->debug = true;
		this->debug_hex = true;
	}
	if(vals.exist({p+"-debug-prec"}))
	{
		this->debug = true;
		this->debug_precision = vals.to_int({p+"-debug-prec"});
	}

	// this->snr_max += 0.0001f; // hack to avoid the miss of the last snr

	if(vals.exist({p+"-threads", "t"}) && vals.to_int({p+"-threads", "t"}) > 0)
		if(vals.exist({p+"-threads", "t"})) this->n_threads = vals.to_int({p+"-threads", "t"});

#ifdef ENABLE_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &this->mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->mpi_rank);

	if(vals.exist({p+"-mpi-comm"})) this->mpi_comm_freq = milliseconds(vals.to_int({p+"-mpi-comm"}));

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

	if (this->snr_range.size())
	{
		std::stringstream snr_range_str;
		snr_range_str << this->snr_range.front() << " -> " << this->snr_range.back() << " dB";
		headers[p].push_back(std::make_pair("SNR range", snr_range_str.str()));
	}

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
