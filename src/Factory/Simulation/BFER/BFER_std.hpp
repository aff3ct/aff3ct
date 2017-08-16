#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "BFER.hpp"
#include "Tools/Codec/Codec.hpp"


namespace aff3ct
{
namespace simulation
{
template <typename B, typename R, typename Q>
class BFER_std;
}
}

namespace aff3ct
{
namespace factory
{
struct BFER_std : BFER
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : BFER::parameters
	{
		parameters() : BFER::parameters() {}

		virtual ~parameters() {}
	};

	template <typename B = int, typename R = float, typename Q = R>
	static simulation::BFER_std<B,R,Q>* build(const parameters &params, tools::Codec<B,Q> &codec);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_sim, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
