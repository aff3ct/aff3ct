#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"
#include "Tools/Header.hpp"
#include "Tools/Arguments_reader.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = int>
struct Factory_interleaver : public Factory
{
	struct Interleaver_parameters
	{
		std::string type       = "RANDOM";
		std::string path       = "";
		int         seed;
		int         n_cols     = 4;     // number of columns of the columns interleaver
		bool        uniform    = false; // set at true to regenerate the interleaver at each new frame
	};

	static module::Interleaver<T>* build(const std::string type,
	                                     const int         size,
	                                     const std::string path     = "",
	                                     const bool        uniform  = false,
	                                     const int         n_cols   = 1,
	                                     const int         seed     = 0,
	                                     const int         n_frames = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, Interleaver_parameters &params, const int seed);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_itl, const Interleaver_parameters& params);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
