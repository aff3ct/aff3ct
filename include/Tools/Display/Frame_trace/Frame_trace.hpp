/*!
 * \file
 * \brief Class tools::Frame_trace.
 */
#ifndef FRAME_TRACE_HPP
#define FRAME_TRACE_HPP

#include <vector>
#include <memory>
#include <iostream>

namespace aff3ct
{
namespace tools
{
/*!
 * \class Frame_trace
 *
 * \brief Traces a frame.
 *
 * \tparam B: type of bits.
 */
template <typename B = int>
class Frame_trace
{
private:
	enum class debug_version : uint8_t { BIT, REAL, HEX };
	std::ostream &stream; /*!< Stream to trace the frame(s). */

	const int n_bits; /*!< Number of bits to display per frame (if n_bits = 0, display all the bits of the frame). */
	const int prec;   /*!< Number of characters used to display the number if real vectors */
	const bool display_indexes = true;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param n_bits: number of bits to display per frame (if n_bits = 0, display all the bits of the frame).
	 * \param stream: stream to trace the frame(s).
	 */
	explicit Frame_trace(const int n_bits = 0, const int prec = 5, std::ostream &stream = std::cout);

	/*!
	 * \brief Display a vector (= a frame) of bits (0 or 1).
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec: a vector to display.
	 * \param ref: a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 *             ref at 1 means a bit at 1 and ref at 0 means a bit at 0
	 */
	template <typename D, class AD = std::allocator<D>, class AB = std::allocator<B>>
	void display_bit_vector(std::vector<D,AD> vec, std::vector<B,AB> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of reals.
	 *
	 * \tparam D: type of the values in the input vector.
	 *
	 * \param vec: a vector to display.
	 * \param ref: a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 *             ref at 1 means a negative value and ref at 0 means a positive value
	 */
	template <typename D, class AD = std::allocator<D>, class AB = std::allocator<B>>
	void display_real_vector(std::vector<D,AD> vec, std::vector<B,AB> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of hexadecimals.
	 *
	 * \tparam D: type of the values in the input vector.
	 *
	 * \param vec: a vector to display.
	 */
	template <typename D, class AD = std::allocator<D>>
	void display_hex_vector(std::vector<D,AD> vec);

	/*!
	 * \brief Display a vector (= a frame) of bits (0 or 1) in 2D.
	 *
	 * \tparam D: type of the bits in the input vector.
	 *
	 * \param vec:       a vector to display.
	 * \param row_width: number of bits in a row (size of the x dimension).
	 * \param ref:       a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 *             ref at 1 means a bit at 1 and ref at 0 means a bit at 0
	 */
	template <typename D, class AD = std::allocator<D>, class AB = std::allocator<B>>
	void display_bit_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of reals in 2D.
	 *
	 * \tparam D: type of the values in the input vector.
	 *
	 * \param vec:       a vector to display.
	 * \param row_width: number of values in a row (size of the x dimension).
	 * \param ref:       a reference vector to compare with the other one (do not perform the comparison if ref is unset).
	 *             ref at 1 means a negative value and ref at 0 means a positive value
	 */
	template <typename D, class AD = std::allocator<D>, class AB = std::allocator<B>>
	void display_real_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref = {});

	/*!
	 * \brief Display a vector (= a frame) of hexadecimals in 2D.
	 *
	 * \tparam D: type of the values in the input vector.
	 *
	 * \param vec:       a vector to display.
	 * \param row_width: number of values in a row (size of the x dimension).
	 */
	template <typename D, class AD = std::allocator<D>>
	void display_hex_vector(std::vector<D,AD> vec, unsigned int row_width);

private:
	template <typename D, class AD = std::allocator<D>, class AB = std::allocator<B>>
	void display_vector(std::vector<D,AD> vec, unsigned int row_width, std::vector<B,AB> ref, debug_version version);

	template <typename D>
	void display_value (D value, debug_version version, B ref = -1);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Display/Frame_trace/Frame_trace.hxx"
#endif

#endif /* FRAME_TRACE_HPP */
