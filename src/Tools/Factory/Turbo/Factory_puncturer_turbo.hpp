#ifndef FACTORY_PUNCTURER_TURBO_HPP
#define FACTORY_PUNCTURER_TURBO_HPP

#include <string>

#include "Tools/Factory/Factory_puncturer.hpp"
#include "Module/Puncturer/Puncturer.hpp"


namespace aff3ct
{
namespace tools
{
template <typename B = int, typename Q = float>
struct Factory_puncturer_turbo : public Factory_puncturer<B,Q>
{
	struct puncturer_parameters_turbo : Factory_puncturer<B,Q>::puncturer_parameters
	{
		virtual ~puncturer_parameters_turbo() {}
		std::string pattern = "111,111,111";
	};

	static module::Puncturer<B,Q>* build(const puncturer_parameters_turbo &params,
	                                     const int                         tail_length,
	                                     const bool                        buffered = true);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, puncturer_parameters_turbo& params,
	                       const int K, const int N, const int N_pct, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_pct, const puncturer_parameters_turbo& params);
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_HPP */
