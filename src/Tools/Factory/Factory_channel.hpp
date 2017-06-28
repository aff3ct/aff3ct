#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>

#include "Tools/Arguments_reader.hpp"
#include "Tools/params.h"
#include "Module/Channel/Channel.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
struct Factory_channel : public Factory
{
	struct channel_parameters
	{
		std::string type         = "AWGN";
		std::string path         = "";
		std::string block_fading = "NO";
	};

	static module::Channel<R>* build(const std::string type,
	                                 const int         N,
	                                 const bool        complex,
	                                 const bool        add_users = false,
	                                 const std::string path      = "",
	                                 const int         seed      = 0,
	                                 const R           sigma     = (R)1,
	                                 const int         n_frames  = 1);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, channel_parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
