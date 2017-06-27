#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"
#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_source : public Factory
{
	static module::Source<B>* build(const std::string type,
	                                const int         K,
	                                const std::string path     = "",
	                                const int         seed     = 0,
	                                const int         n_frames = 1);

	static void build_args(tools::Arguments_reader::arg_map &req_args, tools::Arguments_reader::arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, tools::parameters &params);
	static void group_args(tools::Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
