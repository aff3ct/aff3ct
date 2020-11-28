#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Source/User/Source_user_binary.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Source_user_binary<B>
::Source_user_binary(const int K,
                     const std::string &filename,
                     const bool auto_reset,
                     const bool fifo_mode)
: Source<B>(K),
  source_file(filename.c_str(), std::ios::in | std::ios::binary),
  auto_reset(fifo_mode ? true : auto_reset),
  fifo_mode(fifo_mode),
  done(false),
  n_left(0),
  memblk(K),
  left_bits(CHAR_BIT)
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
::reset()
{
	source_file.clear();
	if (!this->fifo_mode)
		source_file.seekg(0, std::ios::beg);
	if (source_file.fail())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "Could not go back to the beginning of the file.");
	this->done = false;
	this->n_left = 0;
}

template <typename B>
bool Source_user_binary<B>
::is_done() const
{
	return this->done;
}

template <typename B>
void Source_user_binary<B>
::_generate(B *U_K, const size_t frame_id)
{
	if (this->is_done())
		std::fill(U_K, U_K + this->K, (B)0);
	else
	{
		size_t n_bytes_read = 0;
		const size_t n_bytes_needed = (size_t)(this->K - this->n_left + CHAR_BIT -1) / CHAR_BIT; // number of bytes needed

		for (size_t i = 0; i < this->n_left; i++)
			U_K[i] = this->left_bits[i];

		while (n_bytes_read < n_bytes_needed)
		{
			source_file.read(this->memblk.data() + n_bytes_read, n_bytes_needed - n_bytes_read);
			n_bytes_read += source_file.gcount();

			if (source_file.fail())
			{
				if (source_file.eof())
				{
					if (this->auto_reset)
						this->reset();
					else
						break;
				}
				else
					throw tools::runtime_error(__FILE__, __LINE__, __func__, "Unknown error during file reading.");
			}
		}

		if (this->n_left + n_bytes_read * CHAR_BIT <= (size_t)this->K)
		{
			tools::Bit_packer::unpack(this->memblk.data(), U_K + this->n_left, n_bytes_read * CHAR_BIT);
			std::fill(U_K + this->n_left + n_bytes_read * CHAR_BIT, U_K + this->K, (B)0);

			int tmp_n_left = ((int)n_bytes_read * (int)CHAR_BIT) - (this->K - (int)this->n_left);
			this->n_left = tmp_n_left < 0 ? (size_t)0 : (size_t)tmp_n_left;

			if (!this->auto_reset && source_file.eof())
				this->done = true;
		}
		else
		{
			tools::Bit_packer::unpack(this->memblk.data(), U_K + this->n_left, this->K - this->n_left);

			int tmp_n_left = ((int)n_bytes_read * (int)CHAR_BIT) - (this->K - (int)this->n_left);
			this->n_left = tmp_n_left < 0 ? (size_t)0 : (size_t)tmp_n_left;

			if (this->n_left)
			{
				// re-unpack last byte && store into left_bits
				tools::Bit_packer::unpack(this->memblk.data() + n_bytes_needed - 1, this->left_bits.data(), 8);

				// shift the left bits to the beginning of the array
				for (size_t i = 0; i < this->n_left; i++)
					this->left_bits[i] = this->left_bits[i + CHAR_BIT - this->n_left];
			}
		}
	}
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
