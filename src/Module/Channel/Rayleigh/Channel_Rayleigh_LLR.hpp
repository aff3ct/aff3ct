#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#define CHANNEL_RAYLEIGH_LLR_HPP_

#include <vector>

#include "Tools/Algo/Noise/Noise.hpp"
#include "Tools/Algo/Noise/Standard/Noise_std.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_Rayleigh_LLR : public Channel<R> // flat Rayleigh fading channel
{
private:
	const bool complex;
	const bool add_users;
	std::vector<R> gains;
	tools::Noise<R> *noise_generator;

public:
	Channel_Rayleigh_LLR(const int N, const bool complex, tools::Noise<R> *noise_generator = new tools::Noise_std<R>(),
	                     const bool add_users = false, const R sigma = (R)1, const int n_frames = 1,
	                     const std::string name = "Channel_Rayleigh_LLR");
	Channel_Rayleigh_LLR(const int N, const bool complex, const int seed, const bool add_users = false,
	                     const R sigma = (R)1, const int n_frames = 1, const std::string name = "Channel_Rayleigh_LLR");
	virtual ~Channel_Rayleigh_LLR();

	virtual void get_gains(std::vector<R>& gains, const R sigma);

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N); using Channel<R>::add_noise_wg;
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
