#ifndef FACTORY_SIMULATION_HPP
#define FACTORY_SIMULATION_HPP

#ifdef ENABLE_MPI
#include <mpi.h>
#endif
#include <chrono>
#include <string>
#include <sstream>

#include "Tools/Display/bash_tools.h"

#include "Factory/Launcher/Launcher.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Simulation_name;
extern const std::string Simulation_prefix;
struct Simulation : Launcher
{
	class parameters : public Launcher::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// required parameters
		float                     snr_min         = 0.f;
		float                     snr_max         = 0.f;

		// optional parameters
#ifdef ENABLE_MPI
		std::chrono::milliseconds mpi_comm_freq   = std::chrono::milliseconds(1000);
		int                       mpi_rank        = 0;
		int                       mpi_size        = 1;
#endif
		std::chrono::seconds      stop_time       = std::chrono::seconds(0);
		std::string               pyber           = "";
		float                     snr_step        = 0.1f;
		bool                      debug           = false;
		bool                      statistics      = false;
		int                       n_threads       = 1;
		int                       local_seed      = 0;
		int                       global_seed     = 0;
		int                       debug_limit     = 0;
		int                       debug_precision = 2;

		// ---------------------------------------------------------------------------------------------------- METHODS
		virtual ~parameters();
		virtual Simulation::parameters* clone() const;

		// parameters construction
		virtual void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		virtual void store          (const arg_val_map &vals                                           );
		virtual void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

	protected:
		parameters(const std::string &n = Simulation_name, const std::string &p = Simulation_prefix);
	};
};
}
}

#endif /* FACTORY_SIMULATION_HPP */
