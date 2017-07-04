#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/Header.hpp"
#include "Factory.hpp"

#include "Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
struct Factory_puncturer : Factory
{
	struct puncturer_parameters
	{
		virtual ~puncturer_parameters() {}
		std::string type = "NO";
		int K;
		int N;
		int N_pct;
		int n_frames;
	};

	static module::Puncturer<B,Q>* build(const puncturer_parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, puncturer_parameters& params,
	                       const int K, const int N, const int N_pct, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_pct, const puncturer_parameters& params);
};

} /* namespace tools */
} /* namespace aff3ct */

#endif /* FACTORY_PONCTURER_HPP_ */
