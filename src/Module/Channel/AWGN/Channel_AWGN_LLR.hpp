#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include "Tools/Algo/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Algo/Gaussian_noise_generator/Standard/Gaussian_noise_generator_std.hpp"

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
	tools::Gaussian_noise_generator<R> *noise_generator;

public:
	Channel_AWGN_LLR(const int N, tools::Gaussian_gen<R> *noise_generator = new tools::Gaussian_gen_std<R>(),
	                 const bool add_users = false,
	                 const tools::Sigma<R>& noise = tools::Sigma<R>(),
	                 const int n_frames = 1);

	Channel_AWGN_LLR(const int N, const int seed, const bool add_users = false,
	                 const tools::Sigma<R>& noise = tools::Sigma<R>(),
	                 const int n_frames = 1);

	virtual ~Channel_AWGN_LLR();

	void add_noise(const R *X_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise;

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_AWGN_LLR_HPP_ */
