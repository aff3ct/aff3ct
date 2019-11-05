/*!
 * \file
 * \brief Class module::Channel_AWGN_LLR.
 */
#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
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
	tools::Gaussian_gen<R>* gaussian_generator;
	const bool is_autoalloc_gaussian_gen;

public:
	Channel_AWGN_LLR(const int N,
	                 tools::Gaussian_gen<R> &noise_generator,
	                 const bool add_users = false,
	                 const int n_frames = 1);

	explicit Channel_AWGN_LLR(const int N,
	                          const tools::Gaussian_noise_generator_implem implem = tools::Gaussian_noise_generator_implem::STD,
	                          const int seed = 0,
	                          const bool add_users = false,
	                          const int n_frames = 1);

	virtual ~Channel_AWGN_LLR();

	void set_seed(const int seed);

	void add_noise(const R *X_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise;

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_AWGN_LLR_HPP_ */
