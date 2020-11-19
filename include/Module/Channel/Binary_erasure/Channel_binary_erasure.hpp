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
	std::shared_ptr<tools::Event_generator<R>> event_generator;

	using E = typename tools::matching_types<R>::B;

public:
	Channel_binary_erasure(const int N,
	                       const tools::Event_generator<R> &event_generator);

	explicit Channel_binary_erasure(const int N,
	                                const tools::Event_generator_implem implem = tools::Event_generator_implem::STD,
	                                const int seed = 0);

	virtual ~Channel_binary_erasure() = default;

	virtual Channel_binary_erasure<R>* clone() const;

	void set_seed(const int seed);

protected:
	virtual void deep_copy(const Channel_binary_erasure<R>& m);
	void _add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id);
};
}
}

#endif /* CHANNEL_BEC_HPP_ */
