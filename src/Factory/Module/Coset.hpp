#ifndef FACTORY_COSET_HPP
#define FACTORY_COSET_HPP

#include <string>

#include "Module/Coset/Coset.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Coset : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		virtual ~parameters() {}

		int         size     = 0;

		std::string type     = "STD";
		int         n_frames = 1;
	};

	template <typename B1 = int, typename B2 = B1>
	static module::Coset<B1,B2>* build_bit(const parameters &params);

	template <typename B = int, typename R = float>
	static module::Coset<B,R>* build_real(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_dec, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_COSET_HPP */
