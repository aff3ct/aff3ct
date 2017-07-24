#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Source : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		int         K        = 0;

		std::string type     = "RAND";
		std::string path     = "";
		int         n_frames = 1;
		int         seed     = 0;
	};

	template <typename B = int>
	static module::Source<B>* build(const parameters& params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void header(params_list& head_src, const parameters& params);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
