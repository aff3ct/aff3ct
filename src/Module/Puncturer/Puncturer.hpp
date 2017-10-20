/*!
 * \file
 * \brief Punctures a codeword to match a frame size.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <string>
#include <vector>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace pct
	{
		namespace tsk
		{
			enum list { puncture, depuncture, SIZE };
		}

		namespace sck
		{
			namespace puncture   { enum list { X_N1, X_N2, SIZE }; }
			namespace depuncture { enum list { Y_N1, Y_N2, SIZE }; }
		}
	}

/*!
 * \class Puncturer
 *
 * \brief Punctures a codeword to match a frame size.
 *
 * \tparam B: type of the bits in the frames.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Puncturer.
 *
 * Please use Puncturer for inheritance (instead of Puncturer)
 */
template <typename B = int, typename Q = float>
class Puncturer : public Module
{
protected:
	const int K;    /*!< Number of information bits in one frame */
	const int N;    /*!< Size of one frame (= number of bits in one frame) */
	const int N_cw; /*!< Real size of the codeword (Puncturer::N_cw >= Puncturer::N) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param N_cw:     real size of the codeword (Puncturer::N_cw >= Puncturer::N).
	 * \param n_frames: number of frames to process in the Puncturer.
	 * \param name:     Puncturer's name.
	 */
	Puncturer(const int K, const int N, const int N_cw, const int n_frames = 1,
	          const std::string name = "Puncturer")
	: Module(n_frames, name, "Puncturer"), K(K), N(N), N_cw(N_cw)
	{
		if (K <= 0)
		{
			std::stringstream message;
			message << "'K' has to be greater than 0 ('K' = " << K << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N_cw <= 0)
		{
			std::stringstream message;
			message << "'N_cw' has to be greater than 0 ('N_cw' = " << N_cw << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (K > N)
		{
			std::stringstream message;
			message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N > N_cw)
		{
			std::stringstream message;
			message << "'N' has to be smaller or equal to 'N_cw' ('N' = " << N << ", 'N_cw' = " << N_cw << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p1 = this->create_task("puncture");
		auto &p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N_cw * this->n_frames);
		auto &p1s_X_N2 = this->template create_socket_out<B>(p1, "X_N2", this->N      * this->n_frames);
		this->create_codelet(p1, [this, &p1s_X_N1, &p1s_X_N2]() -> int
		{
			this->puncture(static_cast<B*>(p1s_X_N1.get_dataptr()),
			               static_cast<B*>(p1s_X_N2.get_dataptr()));

			return 0;
		});

		auto &p2 = this->create_task("depuncture");
		auto &p2s_Y_N1 = this->template create_socket_in <Q>(p2, "Y_N1", this->N      * this->n_frames);
		auto &p2s_Y_N2 = this->template create_socket_out<Q>(p2, "Y_N2", this->N_cw * this->n_frames);
		this->create_codelet(p2, [this, &p2s_Y_N1, &p2s_Y_N2]() -> int
		{
			this->depuncture(static_cast<Q*>(p2s_Y_N1.get_dataptr()),
			                 static_cast<Q*>(p2s_Y_N2.get_dataptr()));

			return 0;
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Puncturer() {}

	int get_K() const
	{
		return K;
	}

	int get_N() const
	{
		return N;
	}

	int get_N_cw() const
	{
		return N_cw;
	}

	/*!
	 * \brief Punctures a codeword.
	 *
	 * \param X_N1: a complete/valid codeword..
	 * \param X_N2: a punctured codeword (corresponding to the frame size).
	 */
	template <class A = std::allocator<B>>
	void puncture(const std::vector<B,A>& X_N1, std::vector<B,A>& X_N2) const
	{
		if (this->N_cw * this->n_frames != (int)X_N1.size())
		{
			std::stringstream message;
			message << "'X_N1.size()' has to be equal to 'N_cw' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
			        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)X_N2.size())
		{
			std::stringstream message;
			message << "'X_N2.size()' has to be equal to 'N' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->puncture(X_N1.data(), X_N2.data());
	}

	virtual void puncture(const B *X_N1, B *X_N2) const
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_puncture(X_N1 + f * this->N_cw,
			                X_N2 + f * this->N,
			                f);
	}

	/*!
	 * \brief Depunctures a codeword.
	 *
	 * \param Y_N1: a noised and punctured codeword (corresponding to the frame size).
	 * \param Y_N2: a noised and complete/valid codeword.
	 */
	template <class A = std::allocator<Q>>
	void depuncture(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2) const
	{
		if (this->N * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_cw * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->depuncture(Y_N1.data(), Y_N2.data());
	}

	virtual void depuncture(const Q *Y_N1, Q *Y_N2) const
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_depuncture(Y_N1 + f * this->N,
			                  Y_N2 + f * this->N_cw,
			                  f);
	}

protected:
	virtual void _puncture(const B *X_N1, B *X_N2, const int frame_id) const
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#endif /* PUNCTURER_HPP_ */
