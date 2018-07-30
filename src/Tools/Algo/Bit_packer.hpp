
/*!
 * \file
 * \brief Packs and unpacks bits.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef BIT_PACKER_HPP_
#define BIT_PACKER_HPP_

#include <cmath>
#include <sstream>
#include <vector>
#include <climits>
#include <iostream>

#include "Tools/Exception/exception.hpp"

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
	                        const bool msb_to_lsb = false, const bool pack_per_byte = true, const int Nbps = CHAR_BIT)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (vec_in.size() % n_frames)
		{
			std::stringstream message;
			message << "'vec_in.size()' has to be divisible by 'n_frames' ('vec_in.size()' = " << vec_in.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (pack_per_byte)
		{

			if ((vec_out.size() * sizeof(S) * Nbps) < vec_in.size())
			{
				std::stringstream message;
				message << "'vec_out.size()' has to be equal or greater than ('vec_in.size()' / 'sizeof(S)' / 'Nbps') "
				        << "('vec_out.size()' = " << vec_out.size() << ", 'vec_in.size()' = " << vec_in.size()
				        << ", 'sizeof(S)' = " << sizeof(S) << ", 'Nbps' = " << Nbps << ").";
				throw length_error(__FILE__, __LINE__, __func__, message.str());
			}

			Bit_packer::pack(vec_in.data(), (unsigned char*)vec_out.data(), (int)(vec_in.size() / n_frames),
			                 n_frames, msb_to_lsb, Nbps);
		}
		else
		{
			if ((vec_out.size() * Nbps) < vec_in.size())
			{
				std::stringstream message;
				message << "'vec_out.size()' has to be equal or greater than ('vec_in.size()' / 'Nbps') "
				        << "('vec_out.size()' = " << vec_out.size() << ", 'vec_in.size()' = " << vec_in.size()
				        << ", 'Nbps' = " << Nbps << ").";
				throw length_error(__FILE__, __LINE__, __func__, message.str());
			}

			Bit_packer::pack(vec_in.data(), vec_out.data(), (int)(vec_in.size() / n_frames),
			                 n_frames, msb_to_lsb, Nbps);
		}
	}

	/*!
	 * \brief Packs bits into symbols of type S
	 *
	 * \param vec_in:          an input vector of unpacked bits (only 1 bit per element is used to transport data)
	 * \param vec_out:         an output vector of packed bits in symbols of type S
	 * \param n_bits_per_frame:the number of bits that has to be extracted from vec_in and stocked in vec_out
	 * \param msb_to_lsb:      pack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:            the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, typename S>
	static inline void pack(const B *vec_in, S *vec_out, const int n_bits_per_frame,
	                        const int n_frames = 1, const bool msb_to_lsb = false, const int Nbps = CHAR_BIT)
	{
		if (Nbps > (int)(sizeof(S) * CHAR_BIT) || Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and (sizeof(S) * CHAR_BIT). ('Nbps' = " << Nbps
			        << ", 'sizeof(S)' = " << sizeof(S) << ", 'CHAR_BIT' = " << CHAR_BIT << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_symbols_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / Nbps));

		for (auto f = 0; f < n_frames; f++)
			Bit_packer::_pack(vec_in  + f * n_bits_per_frame,
			                  vec_out + f * n_symbols_per_frame,
			                  n_bits_per_frame,
			                  msb_to_lsb,
			                  Nbps);
	}

	/*!
	 * \brief Packs bits per byte in 'vec'.
	 *
	 * \param vec:           a vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param msb_to_lsb:    pack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:          the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>>
	static inline void pack(std::vector<B,AB> &vec, const int n_frames = 1, const bool msb_to_lsb = false,
	                        const int Nbps = CHAR_BIT)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (vec.size() % n_frames)
		{
			std::stringstream message;
			message << "'vec.size()' has to be divisible by 'n_frames' ('vec.size()' = " << vec.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer::pack(vec.data(), (int)(vec.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	template <typename B>
	static inline void pack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool msb_to_lsb = false,
	                        const int Nbps = CHAR_BIT)
	{
		if (Nbps > (int)CHAR_BIT || Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_symbs_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / Nbps));

		unsigned char* symbs_out = (unsigned char*)vec;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer::_pack(vec        + f * n_bits_per_frame,
			                  symbs_out  + f * n_symbs_per_frame,
			                  n_bits_per_frame,
			                  msb_to_lsb,
			                  Nbps);
	}

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
	                          const bool msb_to_lsb = false, const bool unpack_per_byte = true, const int Nbps = CHAR_BIT)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (vec_out.size() % n_frames)
		{
			std::stringstream message;
			message << "'vec_out.size()' has to be divisible by 'n_frames' ('vec_out.size()' = " << vec_out.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (unpack_per_byte)
		{
			if ((vec_in.size() * sizeof(B) * Nbps) < vec_out.size())
			{
				std::stringstream message;
				message << "('vec_in.size()' * 'sizeof(B)' * 'Nbps') has to be equal or greater than 'vec_out.size()' "
				        << "('vec_in.size()' = " << vec_in.size() << ", 'vec_out.size()' = " << vec_out.size()
				        << ", 'sizeof(B)' = " << sizeof(B) << ", 'Nbps' = " << Nbps << ").";
				throw length_error(__FILE__, __LINE__, __func__, message.str());
			}

			Bit_packer::unpack((unsigned char*)vec_in.data(), vec_out.data(), (int)(vec_out.size() / n_frames),
			                   n_frames, msb_to_lsb, Nbps);
		}
		else
		{
			if ((vec_in.size() * Nbps) < vec_out.size())
			{
				std::stringstream message;
				message << "('vec_in.size()' * 'Nbps') has to be equal or greater than 'vec_out.size()' "
				        << "('vec_in.size()' = " << vec_in.size() << ", 'vec_out.size()' = " << vec_out.size()
				        << ", 'Nbps' = " << Nbps << ").";
				throw length_error(__FILE__, __LINE__, __func__, message.str());
			}

			Bit_packer::unpack(vec_in.data(), vec_out.data(), (int)(vec_out.size() / n_frames),
			                   n_frames, msb_to_lsb, Nbps);
		}
	}

	/*!
	 * \brief Unpacks bits from symbols of type S.
	 *
	 * \param vec_in:          an input vector of packed bits in symbols S.
	 * \param vec_out:         an output vector of unpacked bits (1 bit per element)
	 * \param n_bits_per_frame:the number of bits that has to be extracted from vec_in and stocked in vec_out
	 * \param msb_to_lsb:      unpack from MSB to LSB else from LSB to MSB.
 	 * \param Nbps:            the number of bits packed in one symbol (between 1 and (sizeof(S) * CHAR_BIT)).
	 */
	template <typename B, typename S>
	static inline void unpack(const S *vec_in, B *vec_out, const int n_bits_per_frame,
	                          const int n_frames = 1, const bool msb_to_lsb = false, const int Nbps = CHAR_BIT)
	{
		if (Nbps > (int)(sizeof(S) * CHAR_BIT) || Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and (sizeof(S) * CHAR_BIT). ('Nbps' = " << Nbps
			        << ", 'sizeof(S)' = " << sizeof(S) << ", 'CHAR_BIT' = " << CHAR_BIT << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_symbs_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / Nbps));

		for (auto f = 0; f < n_frames; f++)
			Bit_packer::_unpack(vec_in  + f * n_symbs_per_frame,
			                    vec_out + f * n_bits_per_frame,
			                    n_bits_per_frame,
			                    msb_to_lsb,
			                    Nbps);
	}

	/*!
	 * \brief Unpacks bits per byte of vec
	 *
	 * \param vec:        a vector of packed bits (all the bits in each element of vec are used to store bits).
	 * \param msb_to_lsb: unpack from MSB to LSB else from LSB to MSB
 	 * \param Nbps:       the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <typename B, class AB = std::allocator<B>>
	static inline void unpack(std::vector<B,AB> &vec, const int n_frames = 1, const bool msb_to_lsb = false,
	                          const int Nbps = CHAR_BIT)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (vec.size() % n_frames)
		{
			std::stringstream message;
			message << "'vec.size()' has to be divisible by 'n_frames' ('vec.size()' = " << vec.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer::unpack(vec.data(), (int)(vec.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	template <typename B>
	static inline void unpack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool msb_to_lsb = false,
	                          const int Nbps = CHAR_BIT)
	{
		if (Nbps > (int)CHAR_BIT || Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_symbs_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / Nbps));

		unsigned char* bytes = (unsigned char*)vec;
		std::vector<unsigned char> bytes_cpy(n_symbs_per_frame); //TODO: find a way to avoid this allocation

		for (auto f = 0; f < n_frames; f++)
		{
			//TODO: find a way to avoid this copy
			std::copy(&bytes[f * n_symbs_per_frame], &bytes[(f +1) * n_symbs_per_frame], bytes_cpy.begin());

			Bit_packer::_unpack(bytes_cpy.data() + f * n_symbs_per_frame,
			                    vec              + f * n_bits_per_frame,
			                    n_bits_per_frame,
			                    msb_to_lsb,
			                    Nbps);
		}
	}

private:
	template <typename B, typename S>
	static inline void _pack(const B* vec_in, S* symbs_out, const int n_bits, const bool msb_to_lsb, const int Nbps)
	{
		const auto n_symbs = n_bits / Nbps;
		const auto rest = n_bits - n_symbs * Nbps;

		if (!msb_to_lsb) // lsb_to_msb
		{
			for (auto i = 0; i < n_symbs; i++, symbs_out++)
			{
				S symb = 0;
				for (auto j = 0; j < Nbps; j++, vec_in++)
					symb |= ((S)(*vec_in != 0)) << j;
				*symbs_out = symb;
			}

			if (rest != 0)
			{
				S symb = 0;
				for (auto j = 0; j < rest; j++, vec_in++)
					symb |= ((S)(*vec_in != 0)) << j;
				*symbs_out = symb;
			}
		}
		else // msb_to_lsb
		{
			for (auto i = 0; i < n_symbs; i++, symbs_out++)
			{
				S symb = 0;
				for (auto j = 0; j < Nbps; j++, vec_in++)
					symb |= (symb << 1) | (*vec_in != 0);
				*symbs_out = symb << (sizeof(S) * CHAR_BIT - Nbps);
			}

			if (rest != 0)
			{
				S symb = 0;
				for (auto j = 0; j < rest; j++, vec_in++)
					symb |= (symb << 1) | (*vec_in != 0);
				*symbs_out = symb << (sizeof(S) * CHAR_BIT - rest);
			}
		}
	}

	template <typename B, typename S>
	static inline void _unpack(const S *symbs_in, B* vec_out, const int n_bits, const bool msb_to_lsb, const int Nbps)
	{

		const auto n_symbs = n_bits / Nbps;
		const auto rest = n_bits - n_symbs * Nbps;

		if (!msb_to_lsb) // lsb_to_msb
		{
			for (auto i = 0; i < n_symbs; i++, symbs_in++)
				for (auto j = 0; j < Nbps; j++, vec_out++)
					*vec_out = (*symbs_in >> j) & (S)1;

			for (auto j = 0; j < rest; j++, vec_out++)
				*vec_out = (*symbs_in >> j) & (S)1;
		}
		else // msb_to_lsb
		{
			for (auto i = 0; i < n_symbs; i++, symbs_in++)
				for (auto j = 0; j < Nbps; j++, vec_out++)
					*vec_out = (*symbs_in >> (sizeof(S) * CHAR_BIT -1 -j)) & (S)1;

			for (auto j = 0; j < rest; j++, vec_out++)
				*vec_out = (*symbs_in >> (sizeof(S) * CHAR_BIT -1 -j)) & (S)1;
		}
	}
};
}
}

#endif


/* Test code :
#include <random>
#include <string>
#include <vector>
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Algo/Bit_packer.hpp"

int main(int argc, char** argv)
{
	using namespace aff3ct;
	using B = int;
	using S = short;

	std::random_device rd;

	size_t size_in    = (argc >= 2) ? std::stoi(argv[1]) : 16;
	int    Nbps       = (argc >= 3) ? std::stoi(argv[2]) : CHAR_BIT;
	int    n_frames   = (argc >= 4) ? std::stoi(argv[3]) : 1;
	int    msb_to_lsb = (argc >= 5) ? std::stoi(argv[4]) : 0;
	int pack_per_byte = (argc >= 6) ? std::stoi(argv[5]) : 1;
	int    seed       = (argc >= 7) ? std::stoi(argv[6]) : rd();

	size_t size_out   = std::ceil(1.f * size_in / Nbps);

	std::vector<B> vec_in      (size_in  * n_frames);
	std::vector<S> vec_packed  (size_out * n_frames);
	std::vector<B> vec_unpacked(size_in  * n_frames);

	std::mt19937 gen(seed);
	std::bernoulli_distribution d(0.5);

	for (auto& v : vec_in)
		v = d(gen);

	tools::Frame_trace<> ft;

	std::cout << "Vec in" << std::endl;
	ft.display_bit_vector(vec_in, Nbps);

	tools::Bit_packer::pack(vec_in, vec_packed, n_frames, msb_to_lsb, pack_per_byte, Nbps);

	std::cout << "Vec packed" << std::endl;

	if (pack_per_byte)
	{
		std::vector<char> vec_char(vec_packed.size() * sizeof(S));
		std::copy_n((char*)vec_packed.data(), vec_char.size(), vec_char.data());
		ft.display_hex_vector(vec_char, size_out * sizeof(S));
	}
	else
		ft.display_hex_vector(vec_packed, size_out);

	tools::Bit_packer::unpack(vec_packed, vec_unpacked, n_frames, msb_to_lsb, pack_per_byte, Nbps);

	std::cout << "Vec unpacked" << std::endl;
	ft.display_bit_vector(vec_unpacked, Nbps);

	bool same = true;
	for (unsigned i = 0; i < vec_unpacked.size(); i++)
		same &= vec_unpacked[i] == vec_in[i];

	std::cout << "Same in and unpacked = " << same << std::endl;
}
*/