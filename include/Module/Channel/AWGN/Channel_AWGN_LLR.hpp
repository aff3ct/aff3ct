#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include <memory>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Module/Channel/Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_AWGN_LLR : public Channel<R>
{
private:
	const bool add_users;
	std::unique_ptr<tools::Gaussian_gen<R>> noise_generator;

public:
	Channel_AWGN_LLR(const int N, std::unique_ptr<tools::Gaussian_gen<R>>&& noise_generator,
	                 const bool add_users = false,
	                 const tools::Sigma<R>& noise = tools::Sigma<R>(),
	                 const int n_frames = 1);

	explicit Channel_AWGN_LLR(const int N, const int seed = 0, const bool add_users = false,
	                          const tools::Sigma<R>& noise = tools::Sigma<R>(),
	                          const int n_frames = 1);

	virtual ~Channel_AWGN_LLR() = default;

	void add_noise(const R *X_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise;

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_AWGN_LLR_HPP_ */
