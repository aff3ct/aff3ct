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
	Puncturer(const int K, const int N, const int N_cw, const int n_frames = 1);

	/*!
	 * \brief Destructor.
	 */
	virtual ~Puncturer();

	int get_K() const;

	int get_N() const;

	int get_N_cw() const;

	/*!
	 * \brief Punctures a codeword.
	 *
	 * \param X_N1: a complete/valid codeword..
	 * \param X_N2: a punctured codeword (corresponding to the frame size).
	 */
	template <class A = std::allocator<B>>
	void puncture(const std::vector<B,A>& X_N1, std::vector<B,A>& X_N2, const int frame_id = -1) const;

	virtual void puncture(const B *X_N1, B *X_N2, const int frame_id = -1) const;

	/*!
	 * \brief Depunctures a codeword.
	 *
	 * \param Y_N1: a noised and punctured codeword (corresponding to the frame size).
	 * \param Y_N2: a noised and complete/valid codeword.
	 */
	template <class A = std::allocator<Q>>
	void depuncture(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2, const int frame_id = -1) const;

	virtual void depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id = -1) const;

protected:
	virtual void _puncture(const B *X_N1, B *X_N2, const int frame_id) const;

	virtual void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}
#include "Puncturer.hxx"

#endif /* PUNCTURER_HPP_ */
