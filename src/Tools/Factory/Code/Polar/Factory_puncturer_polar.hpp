#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Puncturer/Puncturer.hpp"

#include "../Factory_puncturer.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_puncturer_polar : public Factory_puncturer
{
	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const Factory_puncturer::parameters &params,
	                                     const Frozenbits_generator<B>       &fb_generator);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, Factory_puncturer::parameters& params,
	                       const int K, const int N, const int N_pct, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_pct, const Factory_puncturer::parameters& params);
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
