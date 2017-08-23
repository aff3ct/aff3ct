#ifndef FACTORY_SIMULATION_BFER_HPP_
#define FACTORY_SIMULATION_BFER_HPP_

#include <string>

#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/CRC/CRC.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Monitor/Monitor.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace factory
{
struct BFER : Simulation
{
	static const std::string name;
	static const std::string prefix;

	template <class C>
	struct parameters : Simulation::parameters
	{
		virtual ~parameters() {}

		std::string snr_type         = "EB";
		std::string err_track_path   = "error_tracker";
		bool        err_track_revert = false;
		bool        err_track_enable = false;
		bool        statistics       = false;
		bool        debug            = false;
		bool        coset            = false;
		bool        coded_monitoring = false;
		int         debug_limit      = 0;
		int         debug_precision  = 2;

		Source       ::parameters src;
		CRC          ::parameters crc;
		typename C   ::parameters cdc;
		Modem        ::parameters mdm;
		Channel      ::parameters chn;
		Quantizer    ::parameters qnt;
		Monitor      ::parameters mnt;
		Terminal_BFER::parameters ter;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	template <class C>
	static void store_args(const arg_val_map &vals, parameters<C> &params, const std::string p = prefix);
	template <class C>
	static void make_header(params_list& head_sim, const parameters<C> &params, const bool full = true);
};
}
}

#include "BFER.hxx"

#endif /* FACTORY_SIMULATION_BFER_HPP_ */
