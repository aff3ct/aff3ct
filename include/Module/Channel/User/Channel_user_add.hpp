/*!
 * \file
 * \brief Class module::Channel_user_add.
 */
#ifndef CHANNEL_USER_ADD_HPP_
#define CHANNEL_USER_ADD_HPP_

#include <string>
#include "Module/Channel/Channel.hpp"
#include "Module/Channel/User/Channel_user.hpp"

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
	Channel_user_add(const int N, const std::string &filename, const bool add_users = false);
	virtual ~Channel_user_add() = default;
	virtual Channel_user_add<R>* clone() const;

protected:
	virtual void _add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id);
};
}
}

#endif /* CHANNEL_USER_ADD_HPP_ */
