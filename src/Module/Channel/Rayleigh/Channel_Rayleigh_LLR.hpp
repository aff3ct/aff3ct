#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#define CHANNEL_RAYLEIGH_LLR_HPP_

#include <vector>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"

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
	tools::Gaussian_noise_generator<R> *noise_generator;

public:
	Channel_Rayleigh_LLR(const int N, const bool complex,
	                     tools::Gaussian_gen<R> *noise_generator = new tools::Gaussian_gen_std<R>(),
	                     const bool add_users = false,
	                     const tools::Noise<R>& noise = tools::Noise<R>(),
	                     const int n_frames = 1);

	Channel_Rayleigh_LLR(const int N, const bool complex, const int seed,
	                     const bool add_users = false,
	                     const tools::Noise<R>& noise = tools::Noise<R>(),
	                     const int n_frames = 1);

	virtual ~Channel_Rayleigh_LLR();

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise_wg;

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
