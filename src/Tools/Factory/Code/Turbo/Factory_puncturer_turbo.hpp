#ifndef FACTORY_PUNCTURER_TURBO_HPP
#define FACTORY_PUNCTURER_TURBO_HPP

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../Factory_puncturer.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_puncturer_turbo : public Factory_puncturer
{
	struct parameters : Factory_puncturer::parameters
	{
		virtual ~parameters() {}
		std::string pattern = "111,111,111";
	};

	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const parameters &params,
	                                     const int         tail_length,
	                                     const bool        buffered = true);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params,
	                       const int K, const int N, const int N_pct, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_pct, const parameters& params);
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_HPP */
