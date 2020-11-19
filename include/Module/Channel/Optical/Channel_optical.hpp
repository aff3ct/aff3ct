/*!
 * \file
 * \brief Class module::Channel_optical.
 */
#ifndef CHANNEL_OPTICAL_HPP_
#define CHANNEL_OPTICAL_HPP_

#include <memory>

#include "Tools/Math/Distribution/Distributions.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"
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
	std::shared_ptr<tools::User_pdf_noise_generator<R>> pdf_noise_generator;

public:
	Channel_optical(const int N,
	                const tools::User_pdf_noise_generator<R>& pdf_noise_generator);

	Channel_optical(const int N,
	                const tools::Distributions<R>& dist,
	                const tools::User_pdf_noise_generator_implem implem = tools::User_pdf_noise_generator_implem::STD,
	                const int seed = 0);

	virtual ~Channel_optical() = default;

	virtual Channel_optical<R>* clone() const;

	void set_seed(const int seed);

protected:
	void _add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id);

	virtual void deep_copy(const Channel_optical<R>& m);
};
}
}

#endif /* CHANNEL_OPTICAL_HPP_ */
