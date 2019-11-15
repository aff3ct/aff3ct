/*!
 * \file
 * \brief Class module::Channel_binary_erasure.
 */
#ifndef CHANNEL_BEC_HPP_
#define CHANNEL_BEC_HPP_

#include <memory>

#include "Tools/types.h"
#include "Tools/Algo/Draw_generator/Event_generator/Event_generator.hpp"
#include "Module/Channel/User/Channel_user_be.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_binary_erasure : public Channel<R>
{
protected:
	std::unique_ptr<tools::Event_generator<R>> event_generator;
	const bool is_autoalloc_event_gen;

	using E = typename tools::matching_types<R>::B;

public:
	Channel_binary_erasure(const int N,
	                       const tools::Event_generator<R> &event_generator,
	                       const int n_frames = 1);

	explicit Channel_binary_erasure(const int N,
	                                const tools::Event_generator_implem implem = tools::Event_generator_implem::STD,
	                                const int seed = 0,
	                                const int n_frames = 1);

	virtual ~Channel_binary_erasure() = default;

	void set_seed(const int seed);

protected:
	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_BEC_HPP_ */
