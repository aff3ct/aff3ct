/*!
 * \file
 * \brief Class module::Channel_user.
 */
#ifndef CHANNEL_USER_HPP_
#define CHANNEL_USER_HPP_

#include <vector>
#include <string>

#include "Module/Channel/Channel.hpp"

namespace aff3ct
{
namespace module
{

/*!
 * \class Channel_user
 *
 * \brief The output is directly set by the data read in the given file.
 *
 * \tparam R: type of the reals (floating-point representation) in the Channel.
 */
template <typename R = float>
class Channel_user : public Channel<R>
{
protected:
	const bool add_users;

private:
	std::vector<std::vector<R>> noise_buff;
	int noise_counter;

public:
	Channel_user(const int N, const std::string &filename, const bool add_users = false);
	virtual ~Channel_user() = default;
	virtual Channel_user<R>* clone() const;

	static void read_noise_file(const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);
	static void read_as_text   (const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);
	static void read_as_binary (const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);

protected:
	virtual void _add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id);

	virtual void set_noise(const size_t frame_id);
};
}
}

#endif /* CHANNEL_USER_HPP_ */
