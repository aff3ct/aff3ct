#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <string>

#include "Factory/Module/Interleaver.hpp"

#include "Tools/Codec/Codec_SISO.hpp"

#include "Simulation_BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class Simulation_BFER_ite;
}
}

namespace aff3ct
{
namespace factory
{
struct Simulation_BFER_ite : Simulation_BFER
{
	static const std::string name;
	static const std::string prefix;

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

	template <typename B = int, typename R = float, typename Q = R>
	static simulation::Simulation_BFER_ite<B,R,Q>* build(const parameters &params, tools::Codec_SISO<B,Q> &codec);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
