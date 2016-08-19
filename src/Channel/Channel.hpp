#ifndef CHANNELS_HPP_
#define CHANNELS_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R>
class Channel_interface // please use Channel<R> for inheritance (instead of Channel_interface<R>)
{
public:
	Channel_interface(const std::string name = "Channel_interface") {};
	virtual ~Channel_interface() {};

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;
};

#include "SC_Channel.hpp"

#endif /* CHANNELS_HPP_ */
