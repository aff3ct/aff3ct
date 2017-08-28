#ifndef FACTORY_SIMULATION_EXIT_HPP_
#define FACTORY_SIMULATION_EXIT_HPP_

#include <string>
#include <thread>

#include "../../Module/Monitor/Monitor.hpp"
#include "Factory/Module/Channel/Channel.hpp"
#include "Factory/Module/Modem/Modem.hpp"
#include "Factory/Module/Quantizer/Quantizer.hpp"
#include "Factory/Module/Source/Source.hpp"
#include "Factory/Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Factory/Tools/Display/Terminal/EXIT/Terminal_EXIT.hpp"

#include "../Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R>
class EXIT;
}
}

namespace aff3ct
{
namespace factory
{
struct EXIT : Simulation
{
	static const std::string name;
	static const std::string prefix;

	template <class C>
	struct parameters : Simulation::parameters
	{
		virtual ~parameters() {}

		template <typename B = int, typename R = float>
		simulation::EXIT<C,B,R>* build() const;

		std::string snr_type   = "ES";
		float       sig_a_min  = 0.0f;
		float       sig_a_max  = 5.0f;
		float       sig_a_step = 0.5f;
		bool        debug      = false;

		Source       ::parameters src;
		typename C   ::parameters cdc;
		Modem        ::parameters mdm;
		Channel      ::parameters chn;
		Quantizer    ::parameters qnt;
		Monitor_EXIT ::parameters mnt;
		Terminal_EXIT::parameters ter;
	};

	template <class C, typename B = int, typename R = float>
	static simulation::EXIT<C,B,R>* build(const parameters<C> &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	template <class C>
	static void store_args(const arg_val_map &vals, parameters<C> &params, const std::string p = prefix);
	template <class C>
	static void make_header(params_list& head_sim, const parameters<C> &params, const bool full = true);
};
}
}

#include "EXIT.hxx"

#endif /* FACTORY_SIMULATION_EXIT_HPP_ */
