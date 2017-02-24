#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include "Module/Channel/Channel.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
template <typename R>
struct Factory_channel : public Factory
{
	static Channel<R>* build(const parameters &params, const R& sigma, const int size, const int seed = 0);
};
}

#endif /* FACTORY_CHANNEL_HPP */
