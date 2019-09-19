#ifndef CHANNEL_RAYLEIGH_LLR_USER_HPP_
#define CHANNEL_RAYLEIGH_LLR_USER_HPP_

#include <vector>
#include <string>
#include <memory>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Module/Channel/Channel.hpp"

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
	std::unique_ptr<tools::Gaussian_noise_generator<R>> noise_generator;

	std::vector<R> gains_stock;
	const unsigned gain_occur;
	unsigned current_gain_occur;
	unsigned gain_index;

public:
	Channel_Rayleigh_LLR_user(const int N, const bool complex,
	                          const std::string& gains_filename,
	                          std::unique_ptr<tools::Gaussian_gen<R>>&& noise_generator,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false,
	                          const tools::Noise<R>& noise = tools::Noise<R>(),
	                          const int n_frames = 1);

	Channel_Rayleigh_LLR_user(const int N, const bool complex, const int seed,
	                          const std::string& gains_filename,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false,
	                          const tools::Noise<R>& noise = tools::Noise<R>(),
	                          const int n_frames = 1);

	virtual ~Channel_Rayleigh_LLR_user() = default;

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise_wg;

protected:
	void read_gains(const std::string& gains_filename);

	virtual void check_noise();
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
