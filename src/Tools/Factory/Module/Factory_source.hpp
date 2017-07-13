#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"
#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_source : public Factory
{
	struct parameters
	{
		int         K        = -1;

		std::string type     = "RAND";
		std::string path     = "";
		bool        azcw     = false;
		int         n_frames = 1;
		int         seed     = 0;
	};

	template <typename B = int>
	static module::Source<B>* build(const parameters& params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_src, const parameters& params);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
