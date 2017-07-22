#ifndef FACTORY_TERMINAL_BFER_HPP_
#define FACTORY_TERMINAL_BFER_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/Monitor.hpp"
#include "Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Terminal.hpp"

namespace aff3ct
{
namespace factory
{
struct Terminal_BFER : Terminal
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Terminal::parameters
	{
		int         K     = 0;

		std::string type  = "STD";
		int         N     = 0;
	};

	template <typename B = int>
	static tools::Terminal_BFER<B>* build(const parameters &params, const module::Monitor<B> &monitor,
	                                      const std::chrono::nanoseconds *d_decod_total = nullptr);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const tools::Arguments_reader& ar, parameters& params, const std::string p = prefix);
	static void header(params_list& head_ter, const parameters& params);
};
}
}

#endif /* FACTORY_TERMINAL_HPP_ */
