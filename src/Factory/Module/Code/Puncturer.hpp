#ifndef FACTORY_PONCTURER_HPP_
#define FACTORY_PONCTURER_HPP_

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer : Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}

		int         K        = 0;
		int         N        = 0;

		std::string type     = "NO";
		int         N_cw     = 0;
		int         n_frames = 1;
	};

	template <typename B, typename Q>
	static module::Puncturer<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_pct, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_PONCTURER_HPP_ */
