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

		std::string pattern     = "111,111,111";
		bool        buffered    = true;
		int         tail_length = 0;
	};

	template <typename B = int, typename Q = float>
	static module::Puncturer<B,Q>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters& params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_pct, const parameters& params);
};
}
}

#endif /* FACTORY_PUNCTURER_TURBO_HPP */
