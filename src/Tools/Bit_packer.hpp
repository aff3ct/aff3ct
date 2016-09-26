#ifndef BIT_PACKER_HPP_
#define BIT_PACKER_HPP_

#include "Tools/MIPP/mipp.h"

template <typename B>
struct Bit_packer
{
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

	static void unpack(mipp::vector<B> &vec)
	{
		auto n_elmts = vec.size();
		unsigned char* bytes = (unsigned char*)vec.data();

		auto packed_size = (int)ceil((float)n_elmts / 8.f);
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
