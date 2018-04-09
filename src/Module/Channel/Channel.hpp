/*!
 * \file
 * \brief The Channel is the physical transmission medium.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <string>
#include <vector>

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace chn
	{
		namespace tsk
		{
			enum list { add_noise, add_noise_wg, SIZE };
		}

		namespace sck
		{
			namespace add_noise    { enum list { X_N, Y_N     , SIZE }; }
			namespace add_noise_wg { enum list { X_N, H_N, Y_N, SIZE }; }
		}
	}

/*!
 * \class Channel
 *
 * \brief The Channel is the physical transmission medium.
 *
 * \tparam R: type of the reals (floating-point representation) in the Channel.
 *
 * Please use Channel for inheritance (instead of Channel).
 */
template <typename R = float>
class Channel : public Module
{
protected:
	const int N;     /*!< Size of one frame (= number of bits in one frame) */
	      R   esn0;  /*!< signal power against noise */
	      R   ebn0;  /*!< signal power against noise */
	      R   sigma; /*!< Sigma^2, the noise variance */

	std::vector<R> noise;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Channel.
	 * \param name:     Channel's name.
	 */
	Channel(const int N, const R sigma = -1.f, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel();

	int get_N() const;

	R get_sigma() const;

	const std::vector<R>& get_noise() const;

	virtual void set_sigma(const R sigma);
	virtual void set_noise(const R sigma, const R esn0, const R ebn0);

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise(const std::vector<R,A>& X_N, std::vector<R,A>& Y_N, const int frame_id = -1);

	virtual void add_noise(const R *X_N, R *Y_N, const int frame_id = -1);

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param H_N: the channel gains.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise_wg(const std::vector<R,A>& X_N, std::vector<R,A>& H_N, std::vector<R,A>& Y_N, const int frame_id = -1);

	virtual void add_noise_wg(const R *X_N, R *Y_N, R *H_N, const int frame_id = -1);

protected:
	virtual void _add_noise(const R *X_N, R *Y_N, const int frame_id);

	virtual void _add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id);
};
}
}

#include "Channel.hxx"

#endif /* CHANNEL_HPP_ */
