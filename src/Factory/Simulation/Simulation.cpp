#include <fstream>
#include <algorithm>
#include <rang.hpp>

#ifdef AFF3CT_MPI
#include <mpi.h>
#endif

#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"

#include "Factory/Simulation/Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Simulation_name   = "Simulation";
const std::string aff3ct::factory::Simulation_prefix = "sim";

Simulation::parameters
::parameters(const std::string &name, const std::string &prefix)
: Launcher::parameters(name, Simulation_name, prefix),
  noise(new factory::Noise::parameters(prefix))
{
}

Simulation::parameters* Simulation::parameters
::clone() const
{
	return new Simulation::parameters(*this);

	// if (noise != nullptr) {clone->noise = noise->clone(); }

	// return clone;
}

void Simulation::parameters
::get_description(tools::Argument_map_info &args) const
{
	Launcher::parameters::get_description(args);

	noise->get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Simulation::parameters::";

	tools::add_arg(args, p, class_name+"p+meta",
		tools::Text());

	tools::add_arg(args, p, class_name+"p+stop-time",
		tools::Integer(tools::Positive()),
		tools::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+max-fra,n",
		tools::Integer(tools::Positive()),
		tools::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+crit-nostop",
		tools::None(),
		tools::arg_rank::ADV);

	tools::add_arg(args, p, class_name+"p+dbg",
		tools::None());

	tools::add_arg(args, p, class_name+"p+dbg-hex",
		tools::None());

	tools::add_arg(args, p, class_name+"p+dbg-prec",
		tools::Integer(tools::Positive()));

	tools::add_arg(args, p, class_name+"p+dbg-limit,d",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+dbg-fra",
		tools::Integer(tools::Positive(), tools::Non_zero()));

	tools::add_arg(args, p, class_name+"p+stats",
		tools::None());

	tools::add_arg(args, p, class_name+"p+threads,t",
		tools::Integer(tools::Positive()));

	tools::add_arg(args, p, class_name+"p+seed,S",
		tools::Integer(tools::Positive()));
}

void Simulation::parameters
::store(const tools::Argument_map_value &vals)
{
	using namespace std::chrono;

	Launcher::parameters::store(vals);

	noise->store(vals);

	auto p = this->get_prefix();

	if(vals.exist({p+"-meta"          })) this->meta        =         vals.at    ({p+"-meta"        });
	if(vals.exist({p+"-stop-time"     })) this->stop_time   = seconds(vals.to_int({p+"-stop-time"   }));
	if(vals.exist({p+"-max-fra",   "n"})) this->max_frame   =         vals.to_int({p+"-max-fra", "n"});
	if(vals.exist({p+"-seed",      "S"})) this->global_seed =         vals.to_int({p+"-seed",    "S"});
	if(vals.exist({p+"-stats"         })) this->statistics  = true;
	if(vals.exist({p+"-dbg"           })) this->debug       = true;
	if(vals.exist({p+"-crit-nostop"   })) this->crit_nostop = true;
	if(vals.exist({p+"-dbg-limit", "d"}))
	{
		this->debug = true;
		this->debug_limit = vals.to_int({p+"-dbg-limit", "d"});
	}
	if(vals.exist({p+"-dbg-hex"}))
	{
		this->debug = true;
		this->debug_hex = true;
	}
	if(vals.exist({p+"-dbg-prec"}))
	{
		this->debug = true;
		this->debug_precision = vals.to_int({p+"-dbg-prec"});
	}
	if(vals.exist({p+"-dbg-fra"}))
	{
		this->debug = true;
		this->debug_frame_max = vals.to_int({p+"-dbg-fra"});
	}

	if(vals.exist({p+"-threads", "t"}) && vals.to_int({p+"-threads", "t"}) > 0)
		if(vals.exist({p+"-threads", "t"})) this->n_threads = vals.to_int({p+"-threads", "t"});

#ifdef AFF3CT_MPI
	MPI_Comm_size(MPI_COMM_WORLD, &this->mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->mpi_rank);

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

#ifdef AFF3CT_MULTI_PREC
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

	noise->get_headers(headers, full);

	auto p = this->get_prefix();

	headers[p].push_back(std::make_pair("Seed", std::to_string(this->global_seed)));
	headers[p].push_back(std::make_pair("Statistics", this->statistics ? "on" : "off"));
	headers[p].push_back(std::make_pair("Debug mode", this->debug ? "on" : "off"));
	if (this->debug)
	{
		headers[p].push_back(std::make_pair("Debug precision", std::to_string(this->debug_precision)));
		if (this->debug_limit)
			headers[p].push_back(std::make_pair("Debug limit", std::to_string(this->debug_limit)));
	}

	std::string threads = "unused";
	if (this->n_threads)
		threads = std::to_string(this->n_threads) + " thread(s)";

	headers[p].push_back(std::make_pair("Multi-threading (t)", threads));

#ifdef AFF3CT_MPI
	headers[p].push_back(std::make_pair("MPI size", std::to_string(this->mpi_size)));
#endif
}
