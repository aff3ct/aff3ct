#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <string>

#include "Module/Interleaver/Interleaver.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Interleaver : public Factory
{
	static const std::string name;
	static const std::string prefix;

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

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const tools::Arguments_reader& ar, parameters &params, const std::string p = prefix);
	static void header(params_list& head_itl, const parameters& params);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
