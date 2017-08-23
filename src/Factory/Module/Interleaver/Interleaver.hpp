#ifndef FACTORY_INTERLEAVER_HPP
#define FACTORY_INTERLEAVER_HPP

#include <cstdint>
#include <string>

#include "Tools/Interleaver/Interleaver_core.hpp"

#include "Factory/Tools/Interleaver/Interleaver_core.hpp"

#include "Module/Interleaver/Interleaver.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Interleaver : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		template <typename D = int32_t>
		module::Interleaver<D>* build(const tools::Interleaver_core<>& itl_core) const;

		factory::Interleaver_core::parameters core;
	};

	template <typename D = int32_t>
	static module::Interleaver<D>* build(const tools::Interleaver_core<>& itl_core);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_itl, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_INTERLEAVER_HPP */
