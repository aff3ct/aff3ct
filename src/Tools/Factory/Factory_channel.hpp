#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include "Tools/params.h"

#include "Module/Channel/Channel.hpp"

template <typename R>
struct Factory_channel
{
	static Channel<R>* build(const t_code_param    &code_params, 
	                         const t_channel_param &chan_params, 
	                         const R& sigma,
	                         const int size, 
	                         const int seed = 0);
};

#endif /* FACTORY_CHANNEL_HPP */
