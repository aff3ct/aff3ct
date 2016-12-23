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

#include "Tools/Perf/MIPP/mipp.h"

/*!
 * \class Bit_packer
 *
 * \brief Packs and unpacks bits.
 *
 * \tparam T: type of data to pack/unpack.
 */
template <typename B>
struct Bit_packer
{
	/*!
	 * \brief Packs bits.
	 *
	 * \param vec_in:  an input vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param vec_out: an output vector of packed bits.
	 * \param rev:     reverse the bitpaking storage order.
	 */
	static inline void pack(const mipp::vector<B> &vec_in, mipp::vector<B> &vec_out, const int n_frames = 1,
	                        const bool rev = false)
	{
		assert(vec_out.size() >= static_cast<int>(std::ceil((float)vec_in.size() / (sizeof(B) * 8.f))));
		assert(n_frames > 0);
		assert(vec_in.size() % n_frames == 0);

		const auto n_bits_per_frame  = static_cast<int>(vec_in.size() / n_frames);
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_out = (unsigned char*)vec_out.data();

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec_in .data() + f * n_bits_per_frame,
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
	static inline void pack(mipp::vector<B> &vec, const int n_frames = 1, const bool rev = false)
	{
		assert(n_frames > 0);
		assert(vec.size() % n_frames == 0);

		const auto n_bits_per_frame  = static_cast<int>(vec.size() / n_frames);
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_out = (unsigned char*)vec.data();

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_pack(vec.data() + f * n_bits_per_frame,
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
	static inline void unpack(const mipp::vector<B> &vec_in, mipp::vector<B> &vec_out, const int n_frames = 1,
	                          const bool rev = false)
	{
		assert(vec_in.size() >= static_cast<int>(std::ceil((float)vec_out.size() / (sizeof(B) * 8.f))));
		assert(n_frames > 0);
		assert(vec_out.size() % n_frames == 0);

		const auto n_bits_per_frame  = static_cast<int>(vec_out.size() / n_frames);
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes_in = (unsigned char*)vec_in.data();

		for (auto f = 0; f < n_frames; f++)
			Bit_packer<B>::_unpack(bytes_in       + f * n_bytes_per_frame,
			                       vec_out.data() + f * n_bits_per_frame,
			                       n_bits_per_frame,
			                       rev);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec: a vector of packed bits (all the bits in each element of vec are used to store bits).
	 * \param rev: reverse the bitpaking storage order.
	 */
	static inline void unpack(mipp::vector<B> &vec, const int n_frames = 1, bool rev = false)
	{
		assert(n_frames > 0);
		assert(vec.size() % n_frames == 0);

		const auto n_bits_per_frame  = static_cast<int>(vec.size() / n_frames);
		const auto n_bytes_per_frame = static_cast<int>(std::ceil((float)n_bits_per_frame / 8.f));

		unsigned char* bytes = (unsigned char*)vec.data();
		mipp::vector<unsigned char> bytes_cpy(n_bytes_per_frame); //TODO: find a way to avoid this allocation

		for (auto f = 0; f < n_frames; f++)
		{
			//TODO: find a way to avoid this copy
			std::copy(&bytes[f * n_bytes_per_frame], &bytes[(f +1) * n_bytes_per_frame], bytes_cpy.begin());

			Bit_packer<B>::_unpack(bytes_cpy .data() + f * n_bytes_per_frame,
			                       vec       .data() + f * n_bits_per_frame,
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

#endif
