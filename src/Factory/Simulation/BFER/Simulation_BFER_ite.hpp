#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <string>

#include "Factory/Module/Interleaver.hpp"

#include "Tools/Arguments_reader.hpp"

#include "Simulation_BFER.hpp"

namespace aff3ct
{
namespace factory
{
struct Simulation_BFER_ite : Simulation_BFER
{
	struct parameters : Simulation_BFER::parameters
	{
		parameters()
		: Simulation_BFER::parameters(),
		  itl(new Interleaver::parameters())
		{
		}

		virtual ~parameters()
		{
			if (this->itl != nullptr) { delete this->itl; this->itl = nullptr; }
		}

		int n_ite = 15;

		Interleaver::parameters *itl;
	};

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
