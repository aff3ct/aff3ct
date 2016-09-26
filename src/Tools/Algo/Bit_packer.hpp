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
	 * \param vec: a vector of unpacked bits (only 1 bit per data is used to transport data).
	 */
	static void pack(mipp::vector<B> &vec)
	{
		const auto n_elmts = vec.size();
		unsigned char* bytes = (unsigned char*)vec.data();

		for (auto i = 0; i < n_elmts / 8; i++)
		{
			unsigned char byte = 0;
			for (auto j = 0; j < 8; j++)
			{
				byte <<= 1;
				byte |= (vec[i*8 +j] ? 1 : 0);
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
					byte |= (vec[(n_elmts / 8) * 8 +j] ? 1 : 0);
			}
			bytes[n_elmts / 8] = byte;
		}
	}

	/*!
	 * \brief Unpacks bits.
	 *
	 * \param vec: a vector of packed bits (all the bits in each element of vec are used to store bits).
	 */
	static void unpack(mipp::vector<B> &vec)
	{
		auto n_elmts = vec.size();
		unsigned char* bytes = (unsigned char*)vec.data();

		auto packed_size = (int)std::ceil((float)n_elmts / 8.f);
		mipp::vector<unsigned char> bytes_cpy(packed_size);

		std::copy(&bytes[0], &bytes[packed_size], bytes_cpy.begin());

		for (auto i = 0; i < n_elmts / 8; i++)
		{
			const unsigned char byte = bytes_cpy[i];
			for (auto j = 0; j < 8; j++)
				vec[i * 8 +j] = (byte >> (7 -j)) & 1;
		}

		if (n_elmts % 8 != 0)
		{
			const auto rest = n_elmts % 8;
			const unsigned char byte = bytes_cpy[n_elmts / 8];
			for (auto j = 0; j < 8; j++)
				if (j < rest)
					vec[(n_elmts / 8) * 8 +j] = (byte >> (7 -j)) & 1;
		}
	}
};

#endif
