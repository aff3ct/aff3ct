#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
struct Factory_source : public Factory
{
	struct source_parameters
	{
		std::string type = "RAND";
		std::string path = "";
		bool azcw        = false;
	};

	static module::Source<B>* build(const std::string type,
	                                const int         K,
	                                const std::string path     = "",
	                                const int         seed     = 0,
	                                const int         n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, source_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
