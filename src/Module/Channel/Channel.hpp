/*!
 * \file
 * \brief The Channel is the physical transmission medium.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <cstdint>
#include <vector>
#include <memory>

#include "Tools/Noise/Noise.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace chn
	{
		enum class tsk : uint8_t { add_noise, add_noise_wg, SIZE };

		namespace sck
		{
			enum class add_noise    : uint8_t { X_N, Y_N     , SIZE };
			enum class add_noise_wg : uint8_t { X_N, H_N, Y_N, SIZE };
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
public:
	inline Task&   operator[](const chn::tsk               t) { return Module::operator[]((int)t);                              }
	inline Socket& operator[](const chn::sck::add_noise    s) { return Module::operator[]((int)chn::tsk::add_noise   )[(int)s]; }
	inline Socket& operator[](const chn::sck::add_noise_wg s) { return Module::operator[]((int)chn::tsk::add_noise_wg)[(int)s]; }

protected:
	const int N;          // Size of one frame (= number of bits in one frame)
	std::unique_ptr<tools::Noise<R>> n;   // the current noise to apply to the input signal
	std::vector<R> noise; // vector of the noise applied to the signal

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param noise:    The noise to apply to the signal
	 * \param n_frames: number of frames to process in the Channel.
	 */
	Channel(const int N, const tools::Noise<R>& noise, const int n_frames = 1);

	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Channel.
	 */
	Channel(const int N, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel() = default;

	int get_N() const;

	const std::vector<R>& get_noise() const;

	const tools::Noise<R>* current_noise() const;

	virtual void set_noise(const tools::Noise<R>& noise);

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

	virtual void check_noise(); // check that the noise has the expected type
};
}
}

#include "Module/Channel/Channel.hxx"

#endif /* CHANNEL_HPP_ */
