/*!
 * \file
 * \brief Traces a frame.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef FRAME_TRACE_HPP
#define FRAME_TRACE_HPP

#include "Tools/Perf/MIPP/mipp.h"

namespace aff3ct
{
/*!
 * \class Frame_trace
 *
 * \brief Traces a frame.
 *
 * \tparam B: type of bits.
 */
template <typename B>
class Frame_trace 
{
private:
	enum debug_version { BIT, REAL };
	std::ostream &stream; /*!< Stream to trace the frame(s). */

	const int n_bits; /*!< Number of bits to display per frame (if n_bits = 0, display all the bits of the frame). */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param n_bits: number of bits to display per frame (if n_bits = 0, display all the bits of the frame).
	 * \param stream: stream to trace the frame(s).
	 */
	Frame_trace(const int n_bits = 0, std::ostream &stream = std::cout)
	: stream(stream), n_bits(n_bits)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	~Frame_trace()
	{
	}

	/*!
	 * \brief Display a vector (= a frame) of bits.
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec: a vector to display.
	 * \param ref: a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 */
	template <typename D>
	void display_bit_vector (mipp::vector<D> vec, mipp::vector<B> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of reals.
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec: a vector to display.
	 * \param ref: a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 */
	template <typename D>
	void display_real_vector(mipp::vector<D> vec, mipp::vector<B> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of bits in 2D.
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec:       a vector to display.
	 * \param row_width: number of bits in a row (size of the x dimension).
	 * \param ref:       a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 */
	template <typename D>
	void display_bit_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of reals in 2D.
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec:       a vector to display.
	 * \param row_width: number of bits in a row (size of the x dimension).
	 * \param ref:       a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 */
	template <typename D>
	void display_real_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref = {});

private:
	template <typename D>
	void display_vector(mipp::vector<D> vec, unsigned int row_width, mipp::vector<B> ref, debug_version version);

	template <typename D>
	void display_value (D value, debug_version version                                                         );

	template <typename D>
	void display_value (D value, debug_version version, B ref                                                  );
};
}

#include "Frame_trace.hxx"

#endif /* FRAME_TRACE_HPP */
