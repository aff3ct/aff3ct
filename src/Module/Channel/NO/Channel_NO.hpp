#ifndef CHANNELS_NO_HPP_
#define CHANNELS_NO_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_NO : public Channel<R>
{
public:
	Channel_NO(const int N, const int n_frames = 1, const std::string name = "Channel_NO");
	virtual ~Channel_NO();

	void add_noise(const R *X_N, R *Y_N);
};
}
}

#endif /* CHANNELS_NO_HPP_ */
