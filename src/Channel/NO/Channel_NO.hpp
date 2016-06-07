#ifndef CHANNELS_NO_HPP_
#define CHANNELS_NO_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Channel.hpp"

template <typename B, typename R>
class Channel_NO : public Channel<B,R>
{
public:
	Channel_NO();
	virtual ~Channel_NO();

	void add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N);
};

#endif /* CHANNELS_NO_HPP_ */
