#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Source/User/Source_user_binary.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Source_user_binary<B>
::Source_user_binary(const int K, const std::string filename, const int n_frames)
: Source<B>(K, n_frames), source_file(filename.c_str(), std::ios::in | std::ios::binary)
{
	const std::string name = "Source_user_binary";
	this->set_name(name);

	if (source_file.fail())
	{
		std::stringstream message;
		message << "'filename' file name is not valid: sink file failbit is set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Source_user_binary<B>
::_generate(B *U_K, const int frame_id)
{
	static int n_left = 0; // number of bits that have been left by last call

	int n_bytes_read = 0;
	const int n_bytes_needed = (this->K - n_left + CHAR_BIT - 1) / CHAR_BIT; // number of bytes needed
	std::vector<char> memblk   (n_bytes_needed);
	std::vector<B>    left_bits(CHAR_BIT      ); // to store bits that are left by last call (n_left & n_completing)


	for (auto i = 0; i < n_left; i++)
		U_K[i] = left_bits[i];

	while (n_bytes_read < n_bytes_needed)
	{
		source_file.read(memblk.data() + n_bytes_read, n_bytes_needed - n_bytes_read);
		n_bytes_read += source_file.gcount();

		if (source_file.fail())
		{
			if (source_file.eof())
			{
				source_file.clear();
				source_file.seekg (0, std::ios::beg);
				if (source_file.fail())
					throw tools::runtime_error(__FILE__, __LINE__, __func__, "Could not go back to the beginning of the file.");
			}

			if (source_file.fail())
				throw tools::runtime_error(__FILE__, __LINE__, __func__, "Unknown error during file reading.");
		}
	}

	tools::Bit_packer::unpack(memblk.data(), U_K + n_left, this->K - n_left);

	// (total number of bits read from file) - (bits written into U_K)
	n_left = (n_bytes_needed * CHAR_BIT) - (this->K - n_left);

	// re-unpack last byte && store into left_bits
	tools::Bit_packer::unpack(memblk.data() + n_bytes_needed - 1, left_bits.data(), 1);

	// shift the left bits to the beginning of the array
	for(auto i = 0; i < n_left; i++)
		left_bits[i] = left_bits[i + CHAR_BIT - n_left];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Source_user_binary<B_8>;
template class aff3ct::module::Source_user_binary<B_16>;
template class aff3ct::module::Source_user_binary<B_32>;
template class aff3ct::module::Source_user_binary<B_64>;
#else
template class aff3ct::module::Source_user_binary<B>;
#endif
// ==================================================================================== explicit template instantiation
