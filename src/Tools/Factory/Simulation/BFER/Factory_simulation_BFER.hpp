#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>
#include <thread>

#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Module/Factory_source.hpp"
#include "Tools/Factory/Module/Factory_modem.hpp"
#include "Tools/Factory/Module/Factory_channel.hpp"
#include "Tools/Factory/Module/Factory_monitor.hpp"
#include "Tools/Factory/Module/Code/Factory_encoder.hpp"
#include "Tools/Factory/Module/Code/Factory_decoder.hpp"
#include "Tools/Factory/Simulation/BFER/Factory_terminal_BFER.hpp"

#include "../Factory_simulation.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_BFER : Factory_simulation
{
	struct parameters : Factory_simulation::parameters
	{
		virtual ~parameters() {}

		// ---- simulation
		int         benchs          = 0;
		std::string snr_type        = "EB";
		bool        time_report     = false;
		bool        debug           = false;
		int         debug_limit     = 0;
		int         debug_precision = 5;
	};

	struct chain_parameters : Factory_simulation::chain_parameters
	{
		virtual ~chain_parameters() {}

		Factory_simulation_BFER::parameters *sim = nullptr;
		Factory_source         ::parameters  src;
		Factory_modem          ::parameters  mdm;
		Factory_channel        ::parameters  chn;
		Factory_encoder        ::parameters *enc = nullptr;
		Factory_decoder        ::parameters *dec = nullptr;
		Factory_monitor        ::parameters  mnt;
		Factory_terminal_BFER  ::parameters  ter;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_sim, params_list& head_cde, const parameters& params);
};

}
}

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
