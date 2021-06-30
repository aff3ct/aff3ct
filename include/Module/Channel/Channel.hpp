/*!
 * \file
 * \brief Class module::Channel.
 */
#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>

#include "Tools/Interface/Interface_set_seed.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace chn
	{
		enum class tsk : size_t { add_noise, add_noise_wg, SIZE };

		namespace sck
		{
			enum class add_noise    : size_t { CP, X_N, Y_N     , status };
			enum class add_noise_wg : size_t { CP, X_N, H_N, Y_N, status };
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
class Channel : public Module, public tools::Interface_set_seed
{
public:
	inline Task&   operator[](const chn::tsk               t);
	inline Socket& operator[](const chn::sck::add_noise    s);
	inline Socket& operator[](const chn::sck::add_noise_wg s);

protected:
	const int N;                 // Size of one frame (= number of bits in one frame)
	std::vector<R> noised_data;  // vector of the noise applied to the signal

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N: size of one frame.
	 */
	Channel(const int N);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel() = default;

	virtual Channel<R>* clone() const;

	int get_N() const;

	const std::vector<R>& get_noised_data() const;

	virtual void set_seed(const int seed);

	/*!
	 * \brief Task method that adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise(const std::vector<float,A>& CP, const std::vector<R,A>& X_N, std::vector<R,A>& Y_N,
	               const int frame_id = -1, const bool managed_memory = true);

	void add_noise(const float *CP, const R *X_N, R *Y_N, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Task method that adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param H_N: the channel gains.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise_wg(const std::vector<float,A>& CP, const std::vector<R,A>& X_N, std::vector<R,A>& H_N,
	                  std::vector<R,A>& Y_N, const int frame_id = -1, const bool managed_memory = true);

	void add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const int frame_id = -1,
	                  const bool managed_memory = true);

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual void _add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id);

	virtual void _add_noise_wg(const float *CP, const R *X_N, R *H_N, R *Y_N, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Channel/Channel.hxx"
#endif

#endif /* CHANNEL_HPP_ */
