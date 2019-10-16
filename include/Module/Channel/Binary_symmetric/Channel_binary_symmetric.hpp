#ifndef CHANNEL_BSC_HPP_
#define CHANNEL_BSC_HPP_

#include <memory>

#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"
#include "Tools/Noise/Event_probability.hpp"
#include "Module/Channel/Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_binary_symmetric : public Channel<R>
{
protected:
	std::unique_ptr<tools::Event_generator<R>> event_generator;

	using E = typename tools::matching_types<R>::B; //Event type

public:
	Channel_binary_symmetric(const int N, std::unique_ptr<tools::Event_generator<R>>&& event_generator,
	                         const tools::Event_probability<R>& noise = tools::Event_probability<R>(),
	                         const int n_frames = 1);

	explicit Channel_binary_symmetric(const int N, const int seed = 0,
	                                  const tools::Event_probability<R>& noise = tools::Event_probability<R>(),
	                                  const int n_frames = 1);

	virtual ~Channel_binary_symmetric() = default;

protected:
	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_BSC_HPP_ */
