#ifndef FACTORY_SIMULATION_BFER_STD_HPP_
#define FACTORY_SIMULATION_BFER_STD_HPP_

#include <string>

#include "BFER.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R, typename Q, int CRC, int ITL>
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

	template <class C>
	struct parameters : BFER::parameters<C>
	{
		virtual ~parameters() {}

		template <typename B = int, typename R = float, typename Q = R, int CRC = 0, int ITL = 0>
		simulation::BFER_std<C,B,R,Q,CRC,ITL>* build() const;
	};

	template <class C, typename B = int, typename R = float, typename Q = R, int CRC = 0, int ITL = 0>
	static simulation::BFER_std<C,B,R,Q,CRC,ITL>* build(const parameters<C> &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	template <class C>
	static void store_args(const arg_val_map &vals, parameters<C> &params, const std::string p = prefix);
	template <class C>
	static void make_header(params_list& head_sim, const parameters<C> &params, const bool full = true);
};
}
}

#include "BFER_std.hxx"

#endif /* FACTORY_SIMULATION_BFER_STD_HPP_ */
