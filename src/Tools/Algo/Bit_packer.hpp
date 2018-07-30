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

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Bit_packer
 *
 * \brief Packs and unpacks bits.
 *
 * \tparam B: type of data to pack/unpack.
 */
template <typename B = int>
struct Bit_packer
{
	/*!
	 * \brief Packs bits.
	 *
	 * \param vec_in:     an input vector of unpacked bits (only 1 bit per data is used to transport data) (used as size reference).
	 * \param vec_out:    an output vector of packed bits.
	 * \param msb_to_lsb: pack from MSB to LSB else from LSB to MSB
 	 * \tparam Nbps:      the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <class A = std::allocator<B>>
	static inline void pack(const std::vector<B,A> &vec_in, std::vector<B,A> &vec_out, const int n_frames = 1,
	                        const bool msb_to_lsb = false, int Nbps = CHAR_BIT)
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

		if ((vec_out.size() * sizeof(B) * Nbps) < vec_in.size())
		{
			std::stringstream message;
			message << "('vec_out.size()' * 'sizeof(B)' * 'Nbps') has to be equal or greater than 'vec_in.size()' "
			        << "('vec_out.size()' = " << vec_out.size() << ", 'vec_in.size()' = " << vec_in.size()
			        << ", 'sizeof(B)' = " << sizeof(B) << ", 'Nbps' = " << Nbps << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::pack(vec_in.data(), vec_out.data(), (int)(vec_in.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	static inline void pack(const B *vec_in, B *vec_out, const int n_bits_per_frame,
	                        const int n_frames = 1, const bool msb_to_lsb = false, int Nbps = CHAR_BIT)
	{
		if (Nbps > CHAR_BIT && Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (n_bits_per_frame % Nbps)
		{
			std::stringstream message;
			message << "'n_bits_per_frame' has to be divisible by 'Nbps' ('n_bits_per_frame' = " << n_bits_per_frame
			        << ", 'Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_bytes_per_frame = n_bits_per_frame / Nbps;

		auto bytes_out = (unsigned char*)vec_out;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec_in         + f * n_bits_per_frame,
			                     bytes_out      + f * n_bytes_per_frame,
			                     n_bits_per_frame,
			                     msb_to_lsb,
			                     Nbps);
	}

	/*!
	 * \brief Packs bits.
	 *
	 * \param vec:        a vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param msb_to_lsb: pack from MSB to LSB else from LSB to MSB
 	 * \tparam Nbps:      the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <class A = std::allocator<B>>
	static inline void pack(std::vector<B,A> &vec, const int n_frames = 1, const bool msb_to_lsb = false,
	                        int Nbps = CHAR_BIT)
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

		Bit_packer<B>::pack(vec.data(), (int)(vec.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	static inline void pack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool msb_to_lsb = false,
	                        int Nbps = CHAR_BIT)
	{
		if (Nbps > CHAR_BIT && Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (n_bits_per_frame % Nbps)
		{
			std::stringstream message;
			message << "'n_bits_per_frame' has to be divisible by 'Nbps' ('n_bits_per_frame' = " << n_bits_per_frame
			        << ", 'Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_bytes_per_frame = n_bits_per_frame / Nbps;

		unsigned char* bytes_out = (unsigned char*)vec;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec        + f * n_bits_per_frame,
			                     bytes_out  + f * n_bytes_per_frame,
			                     n_bits_per_frame,
			                     msb_to_lsb,
			                     Nbps);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec_in:     an input vector of packed bits.
	 * \param vec_out:    an output vector of unpacked bits (used as size reference)
	 * \param msb_to_lsb: unpack from MSB to LSB else from LSB to MSB.
 	 * \tparam Nbps:      the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <class A = std::allocator<B>>
	static inline void unpack(const std::vector<B,A> &vec_in, std::vector<B,A> &vec_out, const int n_frames = 1,
	                          const bool msb_to_lsb = false, int Nbps = CHAR_BIT)
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


		if ((vec_in.size() * sizeof(B) * Nbps) < vec_out.size())
		{
			std::stringstream message;
			message << "('vec_in.size()' * 'sizeof(B)' * 'Nbps') has to be equal or greater than 'vec_out.size()' "
			        << "('vec_in.size()' = " << vec_in.size() << ", 'vec_out.size()' = " << vec_out.size()
			        << ", 'sizeof(B)' = " << sizeof(B) << ", 'Nbps' = " << Nbps << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::unpack(vec_in.data(), vec_out.data(), (int)(vec_out.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	static inline void unpack(const B *vec_in, B *vec_out, const int n_bits_per_frame,
	                          const int n_frames = 1, const bool msb_to_lsb = false, int Nbps = CHAR_BIT)
	{
		if (Nbps > CHAR_BIT && Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (n_bits_per_frame % Nbps)
		{
			std::stringstream message;
			message << "'n_bits_per_frame' has to be divisible by 'Nbps' ('n_bits_per_frame' = " << n_bits_per_frame
			        << ", 'Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_bytes_per_frame = n_bits_per_frame / Nbps;

		unsigned char* bytes_in = (unsigned char*)vec_in;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_unpack(bytes_in       + f * n_bytes_per_frame,
			                       vec_out        + f * n_bits_per_frame,
			                       n_bits_per_frame,
			                       msb_to_lsb,
			                       Nbps);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec:        a vector of packed bits (all the bits in each element of vec are used to store bits).
	 * \param msb_to_lsb: unpack from MSB to LSB else from LSB to MSB
 	 * \tparam Nbps:      the number of bits packed in one symbol (between 1 and 8 (CHAR_BIT)).
	 */
	template <class A = std::allocator<B>>
	static inline void unpack(std::vector<B,A> &vec, const int n_frames = 1, bool msb_to_lsb = false,
	                          int Nbps = CHAR_BIT)
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

		Bit_packer<B>::unpack(vec.data(), (int)(vec.size() / n_frames), n_frames, msb_to_lsb, Nbps);
	}

	static inline void unpack(B *vec, const int n_bits_per_frame, const int n_frames = 1, bool msb_to_lsb = false,
	                          int Nbps = CHAR_BIT)
	{
		if (Nbps > CHAR_BIT && Nbps <= 0)
		{
			std::stringstream message;
			message << "'Nbps' must be between 1 and CHAR_BIT. ('Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (n_bits_per_frame % Nbps)
		{
			std::stringstream message;
			message << "'n_bits_per_frame' has to be divisible by 'Nbps' ('n_bits_per_frame' = " << n_bits_per_frame
			        << ", 'Nbps' = " << Nbps << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		const auto n_bytes_per_frame = n_bits_per_frame / Nbps;

		unsigned char* bytes = (unsigned char*)vec;
		std::vector<unsigned char> bytes_cpy(n_bytes_per_frame); //TODO: find a way to avoid this allocation

		for (auto f = 0; f < n_frames; f++)
		{
			//TODO: find a way to avoid this copy
			std::copy(&bytes[f * n_bytes_per_frame], &bytes[(f +1) * n_bytes_per_frame], bytes_cpy.begin());

			Bit_packer<B>::_unpack(bytes_cpy.data() + f * n_bytes_per_frame,
			                       vec              + f * n_bits_per_frame,
			                       n_bits_per_frame,
			                       msb_to_lsb,
			                       Nbps);
		}
	}

private:
	static inline void _pack(const B* vec_in, unsigned char* bytes_out, const int n_bits, const bool msb_to_lsb, int Nbps)
	{
		auto n_bytes = n_bits / Nbps;
		if (!msb_to_lsb) // lsb_to_msb
		{
			for (auto i = 0; i < n_bytes; i++, bytes_out++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < Nbps; j++, vec_in++)
					byte |= ((unsigned char)(*vec_in != 0)) << j;
				*bytes_out = byte;
			}

			const auto rest = n_bits % Nbps;
			if (rest != 0)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < rest; j++, vec_in++)
					byte |= ((unsigned char)(*vec_in != 0)) << j;
				*bytes_out = byte;
			}
		}
		else // msb_to_lsb
		{
			for (auto i = 0; i < n_bytes; i++, bytes_out++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < Nbps; j++, vec_in++)
				{
					byte <<= 1;
					byte |= *vec_in != 0;
				}
				*bytes_out = byte;
			}

			const auto rest = n_bits % Nbps;
			if (rest != 0)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < Nbps; j++, vec_in++)
				{
					byte <<= 1;
					if (j < rest)
						byte |= *vec_in != 0;
				}
				*bytes_out = byte;
			}
		}
	}

	static inline void _unpack(const unsigned char *bytes_in, B* vec_out, const int n_bits, const bool msb_to_lsb, int Nbps)
	{
		auto n_bytes = n_bits / Nbps;
		if (!msb_to_lsb) // lsb_to_msb
		{
			for (auto i = 0; i < n_bytes; i++, bytes_in++)
				for (auto j = 0; j < Nbps; j++, vec_out++)
					*vec_out = (*bytes_in >> j) & 1;

			const auto rest = n_bits % Nbps;
			for (auto j = 0; j < rest; j++, vec_out++)
				*vec_out = (*bytes_in >> j) & 1;
		}
		else // msb_to_lsb
		{
			for (auto i = 0; i < n_bytes; i++, bytes_in++)
				for (auto j = 0; j < Nbps; j++, vec_out++)
					*vec_out = (*bytes_in >> (CHAR_BIT -1 -j)) & 1;

			const auto rest = n_bits % Nbps;
			for (auto j = 0; j < rest; j++, vec_out++)
				*vec_out = (*bytes_in >> (CHAR_BIT -1 -j)) & 1;
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

	std::random_device rd;

    size_t size_in    = (argc >= 2) ? std::stoi(argv[1]) : 16;
    int    Nbps       = (argc >= 3) ? std::stoi(argv[2]) : CHAR_BIT;
    int    n_frames   = (argc >= 4) ? std::stoi(argv[3]) : 1;
    int    msb_to_lsb = (argc >= 5) ? std::stoi(argv[4]) : 0;
    int    seed       = (argc >= 6) ? std::stoi(argv[5]) : rd();
    size_t size_out = std::ceil(1.f * size_in / Nbps / sizeof(B));

    std::vector<B> vec_in (size_in  * n_frames);
    std::vector<B> vec_out(size_out * n_frames);

    std::mt19937 gen(seed);
    std::bernoulli_distribution d(0.5);

    for(auto& v : vec_in)
    	v = d(gen);

    tools::Frame_trace<> ft;

    std::cout << "Vec in" << std::endl;
    ft.display_bit_vector(vec_in, Nbps);

    tools::Bit_packer<int>::pack(vec_in, vec_out, n_frames, msb_to_lsb, Nbps);

    std::cout << "Vec packed" << std::endl;

    std::vector<char> vec_char(vec_out.size() * sizeof(B));
    std::copy_n((char*)vec_out.data(), vec_char.size(), vec_char.data());
    ft.display_hex_vector(vec_char, size_out * sizeof(B));

    tools::Bit_packer<int>::unpack(vec_out, vec_in, n_frames, msb_to_lsb, Nbps);

    std::cout << "Vec unpacked" << std::endl;
    ft.display_bit_vector(vec_in, size_in);
}
*/