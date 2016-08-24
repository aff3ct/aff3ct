#ifndef CHANNELS_HPP_
#define CHANNELS_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename R>
class Channel_interface // please use Channel<R> for inheritance (instead of Channel_interface<R>)
{
protected:
	const int N; // frame size
	      int n_frames;

	const std::string name; // module name

public:
	Channel_interface(const int N, const int n_frames = 1, const std::string name = "Channel_interface")
	: N(N), n_frames(n_frames), name(name) {};
	virtual ~Channel_interface() {};

	virtual void add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N) = 0;

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}
};

#include "SC_Channel.hpp"

#endif /* CHANNELS_HPP_ */
