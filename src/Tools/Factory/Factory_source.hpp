#ifndef FACTORY_SOURCE_HPP
#define FACTORY_SOURCE_HPP

#include <string>

#include "Module/Source/Source.hpp"
#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"

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
		bool        azcw = false;
		int         K;
		int         n_frames;
	};

	static module::Source<B>* build(const source_parameters& params, const int seed = 0);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, source_parameters &params, const int K, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_src, const source_parameters& params);
};
}
}

#endif /* FACTORY_SOURCE_HPP */
