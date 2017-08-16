#ifndef FACTORY_INTERLEAVER_CORE_HPP
#define FACTORY_INTERLEAVER_CORE_HPP

#include <string>

#include "Tools/Interleaver/Interleaver_core.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Interleaver_core : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		int         size     = 0;

		std::string type     = "RANDOM";
		std::string path     = "";
		int         n_cols   = 4; // number of columns of the columns interleaver
		int         n_frames = 1;
		int         seed     = 0;
		bool        uniform  = false; // set at true to regenerate the interleaver at each new frame
	};

	template <typename T = uint32_t>
	static tools::Interleaver_core<T>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_itl, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_INTERLEAVER_CORE_HPP */
