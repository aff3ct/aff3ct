#ifndef CHANNEL_USER_ADD_HPP_
#define CHANNEL_USER_ADD_HPP_

#include <vector>

#include "Channel_user.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Channel_user_add
 *
 * \brief Additive noise to the input with noise read from the given file.
 *
 * \tparam R: type of the reals (floating-point representation) in the Channel.
 */
template <typename R = float>
class Channel_user_add : public Channel_user<R>
{
public:
	Channel_user_add(const int N, const std::string &filename, const bool add_users = false, const int n_frames = 1);
	virtual ~Channel_user_add() = default;

	virtual void add_noise(const R *X_N, R *Y_N, const int frame_id = -1);  using Channel<R>::add_noise;
};
}
}

#endif /* CHANNEL_USER_ADD_HPP_ */
