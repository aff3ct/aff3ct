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
 * \tparam T: type of data to pack/unpack.
 */
template <typename B = int>
struct Bit_packer
{
	/*!
	 * \brief Packs bits.
	 *
	 * \param vec_in:  an input vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param vec_out: an output vector of packed bits.
	 * \param rev:     reverse the bitpaking storage order.
	 */
	template <class A = std::allocator<B>>
	static inline void pack(const std::vector<B,A> &vec_in, std::vector<B,A> &vec_out, const int n_frames = 1,
	                        const bool rev = false)
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
			message << "'vec_in.size()' has to be divisible by 'n_frame' ('vec_in.size()' = " << vec_in.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)vec_out.size() < (int)(std::ceil((float)vec_in.size() / (sizeof(B) * 8.f))))
		{
			std::stringstream message;
			message << "'vec_out.size()' has to be equal or greater than ceil('vec_in.size()' / ('sizeof(B)' * 8.f)) "
			        << "('vec_out.size()' = " << vec_out.size() << ", 'vec_in.size()' = " << vec_in.size()
			        << ", 'sizeof(B)' = " << sizeof(B) << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::pack(vec_in.data(), vec_out.data(), (int)(vec_in.size() / n_frames), n_frames, rev);
	}

	static inline void pack(const B *vec_in, B *vec_out, const int n_bits_per_frame,
	                        const int n_frames = 1, const bool rev = false)
	{
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_out = (unsigned char*)vec_out;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec_in         + f * n_bits_per_frame,
			                     bytes_out      + f * n_bytes_per_frame,
			                     n_bits_per_frame,
			                     rev);
	}

	/*!
	 * \brief Packs bits.
	 *
	 * \param vec: a vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param rev: reverse the bitpaking storage order.
	 */
	template <class A = std::allocator<B>>
	static inline void pack(std::vector<B,A> &vec, const int n_frames = 1, const bool rev = false)
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
			message << "'vec.size()' has to be divisible by 'n_frame' ('vec.size()' = " << vec.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::pack(vec.data(), (int)(vec.size() / n_frames), n_frames, rev);
	}

	static inline void pack(B *vec, const int n_bits_per_frame, const int n_frames = 1, const bool rev = false)
	{
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_out = (unsigned char*)vec;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec        + f * n_bits_per_frame,
			                     bytes_out  + f * n_bytes_per_frame,
			                     n_bits_per_frame,
			                     rev);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec_in:  an input vector of packed bits.
	 * \param vec_out: an output vector of unpacked bits.
	 * \param rev:     reverse the bitpaking storage order.
	 */
	template <class A = std::allocator<B>>
	static inline void unpack(const std::vector<B,A> &vec_in, std::vector<B,A> &vec_out, const int n_frames = 1,
	                          const bool rev = false)
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
			message << "'vec_out.size()' has to be divisible by 'n_frame' ('vec_out.size()' = " << vec_out.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if ((int)vec_in.size() < (int)(std::ceil((float)vec_out.size() / (sizeof(B) * 8.f))))
		{
			std::stringstream message;
			message << "'vec_in.size()' has to be equal or greater than ceil('vec_out.size()' / ('sizeof(B)' * 8.f)) "
			        << "('vec_in.size()' = " << vec_in.size() << ", 'vec_out.size()' = " << vec_out.size()
			        << ", 'sizeof(B)' = " << sizeof(B) << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::unpack(vec_in.data(), vec_out.data(), (int)(vec_out.size() / n_frames), n_frames, rev);
	}

	static inline void unpack(const B *vec_in, B *vec_out, const int n_bits_per_frame,
	                          const int n_frames = 1, const bool rev = false)
	{
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_in = (unsigned char*)vec_in;

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_unpack(bytes_in       + f * n_bytes_per_frame,
			                       vec_out        + f * n_bits_per_frame,
			                       n_bits_per_frame,
			                       rev);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec: a vector of packed bits (all the bits in each element of vec are used to store bits).
	 * \param rev: reverse the bitpaking storage order.
	 */
	template <class A = std::allocator<B>>
	static inline void unpack(std::vector<B,A> &vec, const int n_frames = 1, bool rev = false)
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
			message << "'vec.size()' has to be divisible by 'n_frame' ('vec.size()' = " << vec.size()
			        << ", 'n_frames' = " << n_frames << ").";
			throw length_error(__FILE__, __LINE__, __func__, message.str());
		}

		Bit_packer<B>::unpack(vec.data(), (int)(vec.size() / n_frames), n_frames, rev);
	}

	static inline void unpack(B *vec, const int n_bits_per_frame, const int n_frames = 1, bool rev = false)
	{
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes = (unsigned char*)vec;
		std::vector<unsigned char> bytes_cpy(n_bytes_per_frame); //TODO: find a way to avoid this allocation

		for (auto f = 0; f < n_frames; f++)
		{
			//TODO: find a way to avoid this copy
			std::copy(&bytes[f * n_bytes_per_frame], &bytes[(f +1) * n_bytes_per_frame], bytes_cpy.begin());

			Bit_packer<B>::_unpack(bytes_cpy.data() + f * n_bytes_per_frame,
			                       vec              + f * n_bits_per_frame,
			                       n_bits_per_frame,
			                       rev);
		}
	}

private:
	static inline void _pack(const B* vec_in, unsigned char* bytes_out, const int n_bits, const bool rev = false)
	{
		if (!rev)
		{
			for (auto i = 0; i < n_bits / 8; i++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
					byte |= ((unsigned char)(vec_in[i*8 +j] != 0)) << j;
				bytes_out[i] = byte;
			}

			if (n_bits % 8 != 0)
			{
				const auto rest = n_bits % 8;
				unsigned char byte = 0;
				for (auto j = 0; j < rest; j++)
					byte |= ((unsigned char)(vec_in[(n_bits / 8) * 8 +j] != 0)) << j;
				bytes_out[n_bits / 8] = byte;
			}
		}
		else // reversed order
		{
			for (auto i = 0; i < n_bits / 8; i++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
				{
					byte <<= 1;
					byte |= vec_in[i*8 +j] != 0;
				}
				bytes_out[i] = byte;
			}

			if (n_bits % 8 != 0)
			{
				const auto rest = n_bits % 8;
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
				{
					byte <<= 1;
					if (j < rest)
						byte |= vec_in[(n_bits / 8) * 8 +j] != 0;
				}
				bytes_out[n_bits / 8] = byte;
			}
		}
	}

	static inline void _unpack(const unsigned char *bytes_in, B* vec_out, const int n_bits, const bool rev = false)
	{
		if (!rev)
		{
			for (auto i = 0; i < n_bits / 8; i++)
			{
				const unsigned char byte = bytes_in[i];
				for (auto j = 0; j < 8; j++)
					vec_out[i * 8 +j] = (byte >> j) & 1;
			}

			if (n_bits % 8 != 0)
			{
				const auto rest = n_bits % 8;
				const unsigned char byte = bytes_in[n_bits / 8];
				for (auto j = 0; j < rest; j++)
					vec_out[(n_bits / 8) * 8 +j] = (byte >> j) & 1;
			}
		}
		else // reversed order
		{
			for (auto i = 0; i < n_bits / 8; i++)
			{
				const unsigned char byte = bytes_in[i];
				for (auto j = 0; j < 8; j++)
					vec_out[i * 8 +j] = (byte >> (7 -j)) & 1;
			}

			if (n_bits % 8 != 0)
			{
				const auto rest = n_bits % 8;
				const unsigned char byte = bytes_in[n_bits / 8];
				for (auto j = 0; j < 8; j++)
					if (j < rest)
						vec_out[(n_bits / 8) * 8 +j] = (byte >> (7 -j)) & 1;
			}
		}
	}
};
}
}

#endif
