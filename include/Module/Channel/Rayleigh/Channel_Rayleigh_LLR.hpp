/*!
 * \file
 * \brief Class module::Channel_Rayleigh_LLR.
 */
#ifndef CHANNEL_RAYLEIGH_LLR_HPP_
#define CHANNEL_RAYLEIGH_LLR_HPP_

#include <vector>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Module/Channel/Channel.hpp"

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
	tools::Gaussian_noise_generator<R> *gaussian_generator;
	const bool is_autoalloc_gaussian_gen;

public:
	Channel_Rayleigh_LLR(const int N,
	                     const bool complex,
	                     tools::Gaussian_gen<R> &gaussian_generator,
	                     const tools::Sigma<R> *noise = nullptr,
	                     const bool add_users = false,
	                     const int n_frames = 1);

	Channel_Rayleigh_LLR(const int N,
	                     const bool complex,
	                     const tools::Sigma<R> *noise = nullptr,
	                     const tools::Gaussian_noise_generator_implem implem = tools::Gaussian_noise_generator_implem::STD,
	                     const int seed = 0,
	                     const bool add_users = false,
	                     const int n_frames = 1);

	virtual ~Channel_Rayleigh_LLR();

	void set_seed(const int seed);

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise_wg;

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
