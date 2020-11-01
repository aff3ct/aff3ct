/*!
 * \file
 * \brief Class module::Channel_AWGN_LLR.
 */
#ifndef CHANNEL_AWGN_LLR_HPP_
#define CHANNEL_AWGN_LLR_HPP_

#include <memory>

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
	std::shared_ptr<tools::Gaussian_gen<R>> gaussian_generator;

public:
	Channel_AWGN_LLR(const int N,
	                 const tools::Gaussian_gen<R> &noise_generator,
	                 const bool add_users = false);

	explicit Channel_AWGN_LLR(const int N,
	                          const tools::Gaussian_noise_generator_implem implem = tools::Gaussian_noise_generator_implem::STD,
	                          const int seed = 0,
	                          const bool add_users = false);

	virtual ~Channel_AWGN_LLR() = default;

	virtual Channel_AWGN_LLR<R>* clone() const;

	void set_seed(const int seed);

	void add_noise(const R *X_N, R *Y_N, const int frame_id = -1); using Channel<R>::add_noise;

protected:
	virtual void deep_copy(const Channel_AWGN_LLR<R>& m);

	virtual void check_noise();
};
}
}

#endif /* CHANNEL_AWGN_LLR_HPP_ */
