#ifndef CHANNEL_PDF_HPP_
#define CHANNEL_PDF_HPP_

#include "Tools/Algo/Noise_generator/Noise_generator.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{

/* PDF channel add noise on each symbol from a user defined probability density function.
 * The sigma variable from the inherited class represents the noise power to select in the given noise PDF
 */

template <typename R = float>
class Channel_pdf : public Channel<R>
{
	const bool add_users;
protected:
	tools::Noise_generator<R> *noise_generator;

public:
	Channel_pdf(const int N, tools::Noise_generator<R> *noise_generator, const bool add_users = false,
	            const tools::Noise<R>& noise = tools::ROP<R>(), const int n_frames = 1);

	virtual ~Channel_pdf();

	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);
};
}
}

#endif /* CHANNEL_PDF_HPP_ */
