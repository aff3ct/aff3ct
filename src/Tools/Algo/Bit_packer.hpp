/*!
 * \file
 * \brief Packs and unpacks bits.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef BIT_PACKER_HPP_
#define BIT_PACKER_HPP_

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
	 * \param vec_in: an input vector of unpacked bits (only 1 bit per data is used to transport data).
	 * \param vec_out: an output vector of packed bits.
	 */
	static void pack(const mipp::vector<B> &vec_in, mipp::vector<B> &vec_out, bool rev = false)
	{
		assert(vec_in.size() == vec_out.size());

		const auto n_elmts = vec_in.size();
		unsigned char* bytes = (unsigned char*)vec_out.data();

		if (!rev)
		{
			for (auto i = 0; i < n_elmts / 8; i++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
					byte |= ((unsigned char)(vec_in[i*8 +j] != 0)) << j;
				bytes[i] = byte;
			}

			if (n_elmts % 8 != 0)
			{
				const auto rest = n_elmts % 8;
				unsigned char byte = 0;
				for (auto j = 0; j < rest; j++)
					byte |= ((unsigned char)(vec_in[(n_elmts / 8) * 8 +j] != 0)) << j;
				bytes[n_elmts / 8] = byte;
			}
		}
		else // reversed order
		{
			for (auto i = 0; i < n_elmts / 8; i++)
			{
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
				{
					byte <<= 1;
					byte |= vec_in[i*8 +j] != 0;
				}
				bytes[i] = byte;
			}

			if (n_elmts % 8 != 0)
			{
				const auto rest = n_elmts % 8;
				unsigned char byte = 0;
				for (auto j = 0; j < 8; j++)
				{
					byte <<= 1;
					if (j < rest)
						byte |= vec_in[(n_elmts / 8) * 8 +j] != 0;
				}
				bytes[n_elmts / 8] = byte;
			}
		}
	}

	/*!
	 * \brief Packs bits.
	 *
	 * \param vec: a vector of unpacked bits (only 1 bit per data is used to transport data).
	 */
	static void pack(mipp::vector<B> &vec, bool rev = false)
	{
		Bit_packer<B>::pack(vec, vec, rev);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec_in: an input vector of packed bits.
	 * \param vec_out: an output vector of unpacked bits.
	 */
	static void unpack(const mipp::vector<B> &vec_in, mipp::vector<B> &vec_out, bool rev = false)
	{
		assert(vec_in.size() == vec_out.size());

		auto n_elmts = vec_in.size();
		unsigned char* bytes = (unsigned char*)vec_in.data();

		Bit_packer<B>::_unpack(bytes, vec_out, n_elmts, rev);
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec: a vector of packed bits (all the bits in each element of vec are used to store bits).
	 */
	static void unpack(mipp::vector<B> &vec, bool rev = false)
	{
		auto n_elmts = vec.size();
		unsigned char* bytes = (unsigned char*)vec.data();

		auto packed_size = (int)std::ceil((float)n_elmts / 8.f);
		mipp::vector<unsigned char> bytes_cpy(packed_size); //TODO: find a way to avoid this allocation

		std::copy(&bytes[0], &bytes[packed_size], bytes_cpy.begin()); //TODO: find a way to avoid this copy

		Bit_packer<B>::_unpack(const_cast<const unsigned char*>(bytes_cpy.data()), vec, n_elmts, rev);
	}

private:
	static inline void _unpack(const unsigned char *bytes_in, mipp::vector<B> &vec_out, const int n_elmts, bool rev = false)
	{
		if (!rev)
		{
			for (auto i = 0; i < n_elmts / 8; i++)
			{
				const unsigned char byte = bytes_in[i];
				for (auto j = 0; j < 8; j++)
					vec_out[i * 8 +j] = (byte >> j) & 1;
			}

			if (n_elmts % 8 != 0)
			{
				const auto rest = n_elmts % 8;
				const unsigned char byte = bytes_in[n_elmts / 8];
				for (auto j = 0; j < rest; j++)
					vec_out[(n_elmts / 8) * 8 +j] = (byte >> j) & 1;
			}
		}
		else // reversed order
		{
			for (auto i = 0; i < n_elmts / 8; i++)
			{
				const unsigned char byte = bytes_in[i];
				for (auto j = 0; j < 8; j++)
					vec_out[i * 8 +j] = (byte >> (7 -j)) & 1;
			}

			if (n_elmts % 8 != 0)
			{
				const auto rest = n_elmts % 8;
				const unsigned char byte = bytes_in[n_elmts / 8];
				for (auto j = 0; j < 8; j++)
					if (j < rest)
						vec_out[(n_elmts / 8) * 8 +j] = (byte >> (7 -j)) & 1;
			}
		}
	}
};

#endif
