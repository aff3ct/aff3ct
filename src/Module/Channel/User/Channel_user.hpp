#ifndef CHANNEL_USER_HPP_
#define CHANNEL_USER_HPP_

#include <vector>

#include "../Channel.hpp"

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
public:
	Channel_user(const int N, const std::string &filename, const bool add_users = false, const int n_frames = 1);
	virtual ~Channel_user() = default;

	virtual void add_noise(const R *X_N, R *Y_N, const int frame_id = -1);  using Channel<R>::add_noise;

	static void read_noise_file(const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);
	static void read_as_text   (const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);
	static void read_as_binary (const std::string &filename, const int N, std::vector<std::vector<R>>& noise_buffer);

protected:
	const bool add_users;

	virtual void set_noise(const int frame_id);

private:
	std::vector<std::vector<R>> noise_buff;
	int noise_counter;
};
}
}

#endif /* CHANNEL_USER_HPP_ */
