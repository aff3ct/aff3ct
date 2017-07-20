#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Puncturer/Puncturer.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer_polar : public Puncturer
{
	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const parameters                     &params,
	                                     const tools::Frozenbits_generator<B> &fb_generator);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters& params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_pct, const parameters& params);
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
