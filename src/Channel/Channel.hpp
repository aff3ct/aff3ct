#ifndef CHANNELS_HPP_
#define CHANNELS_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R>
class Channel
{
public:
	Channel(const std::string name = "Channel") {};
	virtual ~Channel() {};

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;
};

#endif /* CHANNELS_HPP_ */
