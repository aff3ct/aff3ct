#ifndef FACTORY_FROZENBITS_GENERATOR_HPP
#define FACTORY_FROZENBITS_GENERATOR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "../../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Frozenbits_generator : public Factory
{
	static const std::string name;
	static const std::string prefix;

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
	static tools::Frozenbits_generator<B>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_fb, const parameters& params);
};
}
}

#endif /* FACTORY_FROZENBITS_GENERATOR_HPP */
