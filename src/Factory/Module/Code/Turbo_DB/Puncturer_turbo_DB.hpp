#ifndef FACTORY_PUNCTURER_TURBO_DB_HPP
#define FACTORY_PUNCTURER_TURBO_DB_HPP

#include <string>

#include "Module/Puncturer/Puncturer.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
struct Puncturer_turbo_DB : public Puncturer
{
	static const std::string name;
	static const std::string prefix;

	struct parameters : Puncturer::parameters
	{
		virtual ~parameters() {}
	};

	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_pct, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_DB_HPP */
