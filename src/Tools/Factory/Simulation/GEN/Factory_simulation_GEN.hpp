#ifndef FACTORY_SIMULATION_GEN_HPP_
#define FACTORY_SIMULATION_GEN_HPP_

#include <string>
#include <thread>

#include "../Factory_simulation.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Tools/Factory/Factory_decoder_common.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_simulation_GEN : Factory_simulation
{
	struct simu_parameters_GEN : simu_parameters
	{
		// ---- simulation

		// ---- code
	};

	template <typename B = int, typename R = float, typename Q = R>
	struct chain_parameters_GEN : chain_parameters
	{
		virtual ~chain_parameters_GEN() {}

		Factory_simulation_GEN         ::simu_parameters_GEN  sim;
		typename Factory_decoder_common::decoder_parameters  *dec = nullptr;
	};

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, simu_parameters_GEN &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_sim, Header::params_list& head_cde, const simu_parameters_GEN& params);
};

}
}

#endif /* FACTORY_SIMULATION_GEN_HPP_ */
