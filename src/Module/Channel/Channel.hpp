#ifndef CHANNELS_HPP_
#define CHANNELS_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename R>
class Channel_interface : public Module // please use Channel<R> for inheritance (instead of Channel_interface<R>)
{
protected:
	const int N; // frame size

public:
	Channel_interface(const int N, const int n_frames = 1, const std::string name = "Channel_interface")
	: Module(n_frames, name), N(N) {}
	virtual ~Channel_interface() {}

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;
};

#include "SC_Channel.hpp"

#endif /* CHANNELS_HPP_ */
