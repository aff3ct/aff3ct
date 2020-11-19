/*!
 * \file
 * \brief Class module::Channel_Rayleigh_LLR_user.
 */
#ifndef CHANNEL_RAYLEIGH_LLR_USER_HPP_
#define CHANNEL_RAYLEIGH_LLR_USER_HPP_

#include <memory>
#include <vector>
#include <string>

#include "Tools/Algo/Draw_generator/Gaussian_noise_generator/Gaussian_noise_generator.hpp"
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
	std::shared_ptr<tools::Gaussian_noise_generator<R>> gaussian_generator;

	std::vector<R> gains_stock;
	const unsigned gain_occur;
	unsigned current_gain_occur;
	unsigned gain_index;

public:
	Channel_Rayleigh_LLR_user(const int N,
	                          const bool complex,
	                          const tools::Gaussian_gen<R> &gaussian_generator,
	                          const std::string& gains_filename,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false);

	Channel_Rayleigh_LLR_user(const int N,
	                          const bool complex,
	                          const std::string& gains_filename,
	                          const tools::Gaussian_noise_generator_implem implem = tools::Gaussian_noise_generator_implem::STD,
	                          const int seed = 0,
	                          const int gain_occurrences = 1,
	                          const bool add_users = false);

	virtual ~Channel_Rayleigh_LLR_user() = default;

	virtual Channel_Rayleigh_LLR_user<R>* clone() const;

	void set_seed(const int seed);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const size_t frame_id);

	virtual void deep_copy(const Channel_Rayleigh_LLR_user<R>& m);

	void read_gains(const std::string& gains_filename);
};
}
}

#endif /* CHANNEL_RAYLEIGH_LLR_HPP_ */
