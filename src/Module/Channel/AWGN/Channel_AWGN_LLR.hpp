#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include "Tools/Algo/Noise/Noise.hpp"
#include "Tools/Algo/Noise/Standard/Noise_std.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_AWGN_LLR : public Channel<R>
{
private:
	const bool add_users;
	tools::Noise<R> *noise_generator;

public:
	Channel_AWGN_LLR(const int N, tools::Noise<R> *noise_generator = new tools::Noise_std<R>(),
	                 const bool add_users = false, const R sigma = (R)1, const int n_frames = 1,
	                 const std::string name = "Channel_AWGN_LLR");

	Channel_AWGN_LLR(const int N, const int seed, const bool add_users = false, const R sigma = (R)1,
	                 const int n_frames = 1, const std::string name = "Channel_AWGN_LLR");

	virtual ~Channel_AWGN_LLR();

	void add_noise(const R *X_N, R *Y_N); using Channel<R>::add_noise;
};
}
}

#endif /* CHANNEL_AWGN_LLR_HPP_ */
