#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>

#include "Module/Channel/Channel.hpp"

#include "../../Factory.hpp"

namespace aff3ct
{
namespace factory
{
struct Channel : public Factory
{
	static const std::string name;
	static const std::string prefix;

	struct parameters
	{
		template <typename R = float>
		module::Channel<R>* build() const;

		int         N            = 0;

		std::string type         = "AWGN";
		std::string path         = "";
		std::string block_fading = "NO";
		bool        add_users    = false;
		bool        complex      = false;
		int         n_frames     = 1;
		int         seed         = 0;
		float       sigma        = -1.f;
	};

	template <typename R = float>
	static module::Channel<R>* build(const parameters &params);

	static void build_args(arg_map &req_args, arg_map &opt_args, const std::string p = prefix);
	static void store_args(const arg_val_map &vals, parameters &params, const std::string p = prefix);
	static void make_header(params_list& head_chn, const parameters& params, const bool full = true);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
