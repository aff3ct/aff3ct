#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#include <chrono>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/auto_cloned_unique_ptr.hpp"
#include "Factory/Tools/Noise/Noise.hpp"
#include "Factory/Launcher/Launcher.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Simulation_name;
extern const std::string Simulation_prefix;
class Simulation : public Launcher
{
public:
	// ------------------------------------------------------------------------------------------------- PARAMETERS
	// required arg
	tools::auto_cloned_unique_ptr<Noise> noise;

	// optional parameters
	std::chrono::seconds stop_time       = std::chrono::seconds(0);
	std::string          meta            = "";
	unsigned             max_frame       = 0;
	bool                 debug           = false;
	bool                 debug_hex       = false;
	bool                 statistics      = false;
	bool                 crit_nostop     = false;
	int                  n_threads       = 1;
	int                  local_seed      = 0;
	int                  global_seed     = 0;
	int                  debug_limit     = 0;
	int                  debug_precision = 2;
	int                  debug_frame_max = 0;
	int                  n_frames        = 1;
#ifdef AFF3CT_MPI
	int                  mpi_rank        = 0;
	int                  mpi_size        = 1;
#endif

	// ---------------------------------------------------------------------------------------------------- METHODS
	virtual ~Simulation() = default;
	virtual Simulation* clone() const;

	// parameters construction
	virtual void get_description(cli::Argument_map_info &args) const;
	virtual void store          (const cli::Argument_map_value &vals);
	virtual void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

protected:
	Simulation(const std::string &n = Simulation_name, const std::string &p = Simulation_prefix);
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
