#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_interleaver : public Factory
{
	struct parameters
	{
		int         size       = 0;

		std::string type       = "RANDOM";
		std::string path       = "";
		int         n_cols     = 4; // number of columns of the columns interleaver
		int         n_frames   = 1;
		int         seed       = 0;
		bool        uniform    = false; // set at true to regenerate the interleaver at each new frame
	};

	template <typename T = int>
	static module::Interleaver<T>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_itl, const parameters& params);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
