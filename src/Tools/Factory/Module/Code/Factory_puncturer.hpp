#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Module/Puncturer/Puncturer.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace tools
{

struct Factory_puncturer : Factory
{
	struct parameters
	{
		virtual ~parameters() {}
		std::string type = "NO";
		int K;
		int N;
		int N_pct;
		int n_frames;
	};

	template <typename B, typename Q>
	static module::Puncturer<B,Q>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params,
	                       const int K, const int N, const int N_pct, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_pct, const parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_PONCTURER_HPP_ */
