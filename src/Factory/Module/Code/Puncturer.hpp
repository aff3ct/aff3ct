#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Module/Puncturer/Puncturer.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer : Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		int         K        = 0;
		int         N        = 0;

		std::string type     = "NO";
		float       R        = -1.f;
		int         N_cw     = 0;
		int         n_frames = 1;
	};

	template <typename B, typename Q>
	static module::Puncturer<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters& params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_pct, const parameters& params);
};
}
}

#endif /* FACTORY_PONCTURER_HPP_ */
