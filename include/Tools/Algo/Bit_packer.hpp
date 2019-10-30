/*!
 * \file
 * \brief Struct tools::Bit_packer.
 */
#ifndef BIT_PACKER_HPP_
#define BIT_PACKER_HPP_

#include <climits>
#include <vector>
#include <memory>

namespace aff3ct
{
namespace tools
{
/*!
 * \class Bit_packer
 *
 * \brief Packs and unpacks bits into symbols
 */
struct Bit_packer
{
	/*!
	 * \brief Packs bits.
	 *
	 * \param vec_in:        an input vector of unpacked bits (only 1 bit per element is used to transport data) (used as size reference).
	 * \param vec_out:       an output vector of packed bits in symbols
	 * \param msb_to_lsb:    pack from MSB to LSB else from LSB to MSB
 	 * \param pack_per_byte: pack bits in each byte (type char) of vec_out, else pack per each symbol (of type S) of vec_out
 	 * \param Nbps:          the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>, typename S, class AS = std::allocator<S>>
	static inline void pack(const std::vector<B,AB> &vec_in, std::vector<S,AS> &vec_out, const int n_frames = 1,
	                        const bool msb_to_lsb = false, const bool pack_per_byte = true, const int Nbps = CHAR_BIT);

	/*!
	 * \brief Packs bits into symbols of type S
	 *
	 * \param vec_in:           an input vector of unpacked bits (only 1 bit per element is used to transport data)
	 * \param vec_out:          an output vector of packed bits in symbols of type S
	 * \param n_bits_per_frame: the number of bits that has to be extracted from vec_in and stocked in vec_out
	 * \param msb_to_lsb:       pack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:             the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, typename S>
	static inline void pack(const B *vec_in, S *vec_out, const int n_bits_per_frame,
	                        const int n_frames = 1, const bool msb_to_lsb = false, const int Nbps = CHAR_BIT);

	/*!
	 * \brief Packs bits per byte in 'vec'.
	 *
	 * \param vec:           a vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param msb_to_lsb:    pack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:          the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>>
	static inline void pack(std::vector<B,AB> &vec, const int n_frames = 1, const bool msb_to_lsb = false,
	                        const int Nbps = CHAR_BIT);

	template <typename B>
	static inline void pack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool msb_to_lsb = false,
	                        const int Nbps = CHAR_BIT);

	/*!
	 * \brief Unpacks bits from symbols.
	 *
	 * \param vec_in:          an input vector of packed bits.
	 * \param vec_out:         an output vector of unpacked bits (1 bit per element) (used as size reference)
	 * \param msb_to_lsb:      unpack from MSB to LSB else from LSB to MSB.
 	 * \param unpack_per_byte: unpack symbol in each byte (type char) of vec_out, else unpack per each symbol (of type S) of vec_in
 	 * \param Nbps:            the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>, typename S, class AS = std::allocator<S>>
	static inline void unpack(const std::vector<S,AS> &vec_in, std::vector<B,AB> &vec_out, const int n_frames = 1,
	                          const bool msb_to_lsb = false, const bool unpack_per_byte = true, const int Nbps = CHAR_BIT);

	/*!
	 * \brief Unpacks bits from symbols of type S.
	 *
	 * \param vec_in:           an input vector of packed bits in symbols S.
	 * \param vec_out:          an output vector of unpacked bits (1 bit per element)
	 * \param n_bits_per_frame: the number of bits that has to be extracted from vec_in and stocked in vec_out
	 * \param msb_to_lsb:       unpack from MSB to LSB else from LSB to MSB.
 	 * \param Nbps:             the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, typename S>
	static inline void unpack(const S *vec_in, B *vec_out, const int n_bits_per_frame,
	                          const int n_frames = 1, const bool msb_to_lsb = false, const int Nbps = CHAR_BIT);

	/*!
	 * \brief Unpacks bits per byte of vec
	 *
	 * \param vec:        a vector of packed bits (all the bits in each element of vec are used to store bits).
	 * \param msb_to_lsb: unpack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:       the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>>
	static inline void unpack(std::vector<B,AB> &vec, const int n_frames = 1, const bool msb_to_lsb = false,
	                          const int Nbps = CHAR_BIT);

	template <typename B>
	static inline void unpack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool msb_to_lsb = false,
	                          const int Nbps = CHAR_BIT);

private:
	template <typename B, typename S>
	static inline void _pack(const B* vec_in, S* symbs_out, const int n_bits, const bool msb_to_lsb, const int Nbps);

	template <typename B, typename S>
	static inline void _unpack(const S *symbs_in, B* vec_out, const int n_bits, const bool msb_to_lsb, const int Nbps);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Algo/Bit_packer.hxx"
#endif

#endif