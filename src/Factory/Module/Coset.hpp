#ifndef FACTORY_COSET_HPP
#define FACTORY_COSET_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"

#include "Module/Coset/Coset.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Coset : public Factory
{
	struct parameters
	{
		virtual ~parameters() {}

		int         size     = 0;

		std::string type     = "STD";
		int         n_frames = 1;
	};

	template <typename B = int, typename BR = B>
	static module::Coset<B,BR>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args);
	static void store_args(const tools::Arguments_reader& ar, parameters &params);
	static void group_args(arg_grp& ar);

	static void header(params_list& head_dec, const parameters& params);
};
}
}

#endif /* FACTORY_COSET_HPP */
