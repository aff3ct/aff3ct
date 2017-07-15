#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#define FACTORY_FROZENBITS_GENERATOR_HPP

#include "Tools/Arguments_reader.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_frozenbits_generator : public Factory
{
	struct parameters
	{
		int         K       = -1;
		int         N_cw    = -1;

		std::string type    = "GA";
		std::string path_fb = "../conf/cde/awgn_polar_codes/TV";
		std::string path_pb = "../lib/polar_bounds/bin/polar_bounds";
		float       sigma   = -1.f;
	};

	template <typename B = int>
	static Frozenbits_generator<B>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_fb, const parameters& params);
};
}
}

#endif /* FACTORY_FROZENBITS_GENERATOR_HPP */
