#ifndef CHANNEL_RAYLEIGH_LLR_USER_HPP_
#define CHANNEL_RAYLEIGH_LLR_USER_HPP_

#include <vector>
#include <string>

#include "Tools/Algo/Noise/Noise.hpp"
#include "Tools/Algo/Noise/Standard/Noise_std.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_Rayleigh_LLR_user : public Channel<R>
{
protected:
	const bool complex;
	const bool add_users;
	std::vector<R> gains;
	tools::Noise<R> *noise_generator;

	std::vector<R> gains_stock;
	const unsigned gain_occur;
	unsigned current_gain_occur;
	unsigned gain_index;

public:
	Channel_Rayleigh_LLR_user(const int N, const bool complex, const std::string& gains_filename,
	                          const int gain_occurrences = 1,
	                          tools::Noise<R> *noise_generator = new tools::Noise_std<R>(),
	                          const bool add_users = false, const R sigma = (R)1, const int n_frames = 1,
	                          const std::string name = "Channel_Rayleigh_LLR_user");
	Channel_Rayleigh_LLR_user(const int N, const bool complex, const int seed, const std::string& gains_filename,
	                          const int gain_occurrences = 1, const bool add_users = false, const R sigma = (R)1,
	                          const int n_frames = 1, const std::string name = "Channel_Rayleigh_LLR_user");
	virtual ~Channel_Rayleigh_LLR_user();

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N); using Channel<R>::add_noise_wg;

protected:
	void read_gains(const std::string& gains_filename);
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
