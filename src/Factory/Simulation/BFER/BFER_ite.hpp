#ifndef FACTORY_SIMULATION_BFER_ITE_HPP_
#define FACTORY_SIMULATION_BFER_ITE_HPP_

#include <string>

#include "BFER.hpp"
#include "Factory/Module/Interleaver.hpp"

#include "Tools/Codec/Codec_SISO.hpp"


namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class BFER_ite;
}
}

namespace aff3ct
{
namespace factory
{
struct BFER_ite : BFER
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : BFER::parameters
	{
		parameters()
		: BFER::parameters(),
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
	static simulation::BFER_ite<B,R,Q>* build(const parameters &params, tools::Codec_SISO<B,Q> &codec);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sim, const parameters& params);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_ITE_HPP_ */
