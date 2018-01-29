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
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"

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
	Channel(const int N, const R sigma = -1.f, const int n_frames = 1)
	: Module(n_frames), N(N), sigma(sigma), noise(this->N * this->n_frames, 0)
	{
		const std::string name = "Channel";
		this->set_name(name);
		this->set_short_name(name);

		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p1 = this->create_task("add_noise");
		auto &p1s_X_N = this->template create_socket_in <R>(p1, "X_N", this->N * this->n_frames);
		auto &p1s_Y_N = this->template create_socket_out<R>(p1, "Y_N", this->N * this->n_frames);
		this->create_codelet(p1, [this, &p1s_X_N, &p1s_Y_N]() -> int
		{
			this->add_noise(static_cast<R*>(p1s_X_N.get_dataptr()),
			                static_cast<R*>(p1s_Y_N.get_dataptr()));

			return 0;
		});

		auto &p2 = this->create_task("add_noise_wg");
		auto &p2s_X_N = this->template create_socket_in <R>(p2, "X_N", this->N * this->n_frames);
		auto &p2s_H_N = this->template create_socket_out<R>(p2, "H_N", this->N * this->n_frames);
		auto &p2s_Y_N = this->template create_socket_out<R>(p2, "Y_N", this->N * this->n_frames);
		this->create_codelet(p2, [this, &p2s_X_N, &p2s_H_N, &p2s_Y_N]() -> int
		{
			this->add_noise_wg(static_cast<R*>(p2s_X_N.get_dataptr()),
			                   static_cast<R*>(p2s_H_N.get_dataptr()),
			                   static_cast<R*>(p2s_Y_N.get_dataptr()));

			return 0;
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Channel()
	{
	}

	int get_N() const
	{
		return this->N;
	}

	R get_sigma() const
	{
		return this->sigma;
	}

	const std::vector<R>& get_noise() const
	{
		return noise;
	}

	virtual void set_sigma(const R sigma)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->sigma = sigma;
	}

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise(const std::vector<R,A>& X_N, std::vector<R,A>& Y_N, const int frame_id = -1)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (X_N.size() != Y_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'Y_N.size()' ('X_N.size()' = "
			        << X_N.size() << ", 'Y_N.size()' = " << Y_N.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)X_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'N' * 'n_frames' ('X_N.size()' = "
			        << X_N.size() << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N.size())
		{
			std::stringstream message;
			message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = "
			        << Y_N.size() << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (frame_id != -1 && frame_id >= this->n_frames)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = " 
			        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->add_noise(X_N.data(), Y_N.data(), frame_id);
	}

	virtual void add_noise(const R *X_N, R *Y_N, const int frame_id = -1)
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		for (auto f = f_start; f < f_stop; f++)
			this->_add_noise(X_N + f * this->N,
			                 Y_N + f * this->N,
			                 f);
	}

	/*!
	 * \brief Adds the noise to a perfectly clear signal.
	 *
	 * \param X_N: a perfectly clear message.
	 * \param H_N: the channel gains.
	 * \param Y_N: a noisy signal.
	 */
	template <class A = std::allocator<R>>
	void add_noise_wg(const std::vector<R,A>& X_N, std::vector<R,A>& H_N, std::vector<R,A>& Y_N, const int frame_id = -1)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (X_N.size() != Y_N.size() || Y_N.size() != H_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'Y_N.size()' and 'H_N.size()' ('X_N.size()' = "
			        << X_N.size() << ", 'Y_N.size()' = " << Y_N.size() << ", 'H_N.size()' = " << H_N.size() << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)X_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'N' * 'n_frames' ('X_N.size()' = "
			        << X_N.size() << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N.size())
		{
			std::stringstream message;
			message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = "
			        << Y_N.size() << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)H_N.size())
		{
			std::stringstream message;
			message << "'H_N.size()' has to be equal to 'N' * 'n_frames' ('H_N.size()' = "
			        << H_N.size() << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (frame_id != -1 && frame_id >= this->n_frames)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = " 
			        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->add_noise_wg(X_N.data(), H_N.data(), Y_N.data(), frame_id);
	}

	virtual void add_noise_wg(const R *X_N, R *Y_N, R *H_N, const int frame_id = -1)
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		for (auto f = f_start; f < f_stop; f++)
			this->_add_noise_wg(X_N + f * this->N,
			                    H_N + f * this->N,
			                    Y_N + f * this->N,
			                    f);
	}

protected:
	virtual void _add_noise(const R *X_N, R *Y_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _add_noise_wg(const R *X_N, R *H_N, R *Y_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#endif /* CHANNEL_HPP_ */
