#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include <string>

#include "Module/Channel/Channel.hpp"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
struct Factory_channel : public Factory
{
	static module::Channel<R>* build(const std::string type,
	                                 const int         N,
	                                 const R           sigma,
	                                 const bool        complex,
	                                 const std::string path     = "",
	                                 const int         seed     = 0,
	                                 const int         n_frames = 1);
};
}
}

#endif /* FACTORY_CHANNEL_HPP */
