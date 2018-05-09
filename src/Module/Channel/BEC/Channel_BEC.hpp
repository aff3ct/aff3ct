#ifndef CHANNEL_BEC_HPP_
#define CHANNEL_BEC_HPP_

#include <vector>
#include <mipp.h>

#include <random>

#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_BEC : public Channel<R>
{
protected:
	tools::Event_generator<R> *event_generator;

public:
	Channel_BEC(const int N, tools::Event_generator<R> *event_generator = new tools::Event_generator_std<R>(),
	            const tools::Event_probability<R>& noise = tools::Event_probability<R>(),
	            const int n_frames = 1);

	Channel_BEC(const int N, const int seed,
	            const tools::Event_probability<R>& noise = tools::Event_probability<R>(),
	            const int n_frames = 1);
	virtual ~Channel_BEC() = default;

protected:
	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);
	virtual void        check_noise    ();
};
}
}

#endif /* CHANNEL_BEC_HPP_ */
