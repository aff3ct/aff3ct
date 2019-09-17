#ifndef CHANNEL_OPTICAL_HPP_
#define CHANNEL_OPTICAL_HPP_

#include <memory>

#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/Received_optical_power.hpp"
#include "Module/Channel/Channel.hpp"

namespace aff3ct
{
namespace module
{

/* Optical channel is for optical communications.
 * There is a specific noise applied on sent bit at 0, and another one to 1.
 * The sigma variable from the inherited class represents the received optical power (ROP)
*/

template <typename R = float>
class Channel_optical : public Channel<R>
{
protected:
	std::unique_ptr<tools::User_pdf_noise_generator<R>> noise_generator;

public:
	Channel_optical(const int N, std::unique_ptr<tools::User_pdf_noise_generator<R>>&& noise_generator,
	                const tools::Noise<R>& noise = tools::ROP<R>(), const int n_frames = 1);

	virtual ~Channel_optical() = default;

	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);

protected:
	virtual void check_noise();
};
}
}

#endif /* CHANNEL_OPTICAL_HPP_ */
