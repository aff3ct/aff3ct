#ifndef CHANNELS_HPP_
#define CHANNELS_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Channel
{
public:
	Channel() {};
	virtual ~Channel() {};

	virtual void add_noise(const mipp::vector<B>& X_N, mipp::vector<R>& Y_N) = 0;
};

#endif /* CHANNELS_HPP_ */
