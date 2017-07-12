#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"
#include "Tools/Header.hpp"
#include "Module/Channel/Channel.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_channel : public Factory
{
	struct parameters
	{
		std::string type         = "AWGN";
		std::string path         = "";
		std::string block_fading = "NO";
		bool        add_users    = false;
		bool        complex;
		int         N;
		int         n_frames;
	};

	template <typename R = float>
	static module::Channel<R>* build(const parameters &params,
	                                 const int         seed  = 0,
	                                 const R           sigma = (R)1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params,
	                       const int N, const bool complex, const bool add_users, const int n_frames = 1);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(Header::params_list& head_chn, const parameters& params);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
