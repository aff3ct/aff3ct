#ifndef CHANNELS_USER_HPP_
#define CHANNELS_USER_HPP_

#include <vector>
#include <mipp.h>

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_user : public Channel<R>
{
private:
	const bool add_users;
	const bool additive_noise;
	mipp::vector<mipp::vector<R>> noise_buff;
	int noise_counter;

public:
	Channel_user(const int N, const std::string &filename, const bool add_users = false, const bool additive_noise = true, const int n_frames = 1);
	virtual ~Channel_user() = default;

	void add_noise(const R *X_N, R *Y_N, const int frame_id = -1);  using Channel<R>::add_noise;

protected:
	void open_as_text  (const std::string& filename);
	void open_as_binary(const std::string& filename);
};
}
}

#endif /* CHANNELS_USER_HPP_ */
