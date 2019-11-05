/*!
 * \file
 * \brief Class module::Channel_Rayleigh_LLR_user.
 */
#ifndef CHANNEL_RAYLEIGH_LLR_USER_HPP_
#define CHANNEL_RAYLEIGH_LLR_USER_HPP_

#include <vector>
#include <string>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
#include "Tools/Noise/Sigma.hpp"
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
	tools::Gaussian_noise_generator<R> *gaussian_generator;
	const bool is_autoalloc_gaussian_gen;

	std::vector<R> gains_stock;
	const unsigned gain_occur;
	unsigned current_gain_occur;
	unsigned gain_index;

public:
	Channel_Rayleigh_LLR_user(const int N,
	                          const bool complex,
	                          tools::Gaussian_gen<R> &gaussian_generator,
	                          const std::string& gains_filename,
	                          const tools::Sigma<R> *noise = nullptr,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false,
	                          const int n_frames = 1);

	Channel_Rayleigh_LLR_user(const int N,
	                          const bool complex,
	                          const std::string& gains_filename,
	                          const tools::Sigma<R> *noise = nullptr,
	                          const tools::Gaussian_noise_generator_implem implem = tools::Gaussian_noise_generator_implem::STD,
	                          const int seed = 0,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false,
	                          const int n_frames = 1);

	virtual ~Channel_Rayleigh_LLR_user();

	void set_seed(const int seed);

	virtual void add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise_wg;

protected:
	void read_gains(const std::string& gains_filename);

	virtual void check_noise();
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
