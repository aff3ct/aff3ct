#include <sstream>
#include <cstdint>
#include <ios>

#include "Tools/Algo/Bit_packer.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Sink/User/Sink_user_binary.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Sink_user_binary<B>
::Sink_user_binary(const int K, const std::string &filename)
: Sink<B>(K),
  filename(filename),
  sink_file(filename.c_str(), std::ios::out | std::ios::binary),
  chunk(K),
  reconstructed_buffer(CHAR_BIT),
  n_left(0)
{
	const std::string name = "Sink_user_binary";
	this->set_name(name);

	if (this->K < CHAR_BIT)
	{
		std::stringstream message;
		message << "'K' has to be greater or equal to 'CHAR_BIT' ('K' = " <<  this->K
		        << ", 'CHAR_BIT' = " <<  CHAR_BIT << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (sink_file.fail())
	{
		std::stringstream message;
		message << "'filename' file name is not valid: sink file failbit is set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
void Sink_user_binary<B>
::reset()
{
	sink_file.close();
	sink_file.open(this->filename.c_str(), std::ios::out | std::ios::binary);
	if (sink_file.fail())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "Could not go back to the beginning of the file.");
	this->n_left = 0;
}

template <typename B>
void Sink_user_binary<B>
::_send(const B *V, const size_t frame_id)
{
	size_t n_completing  = (CHAR_BIT - this->n_left) % CHAR_BIT; // number of bits that are needed to complete one byte
	char reconstructed_byte;                                     // to store reconstructed byte (n_left & n_completing)

	if (sink_file.fail())
	{
		std::stringstream message;
		message << "'filename' file name is not valid: sink file failbit is set.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->n_left != 0)
	{
		for (size_t i = 0; i < n_completing; i++) // completing byte with n_completing first bits of V
			this->reconstructed_buffer[this->n_left +i] = V[i];

		tools::Bit_packer::pack(this->reconstructed_buffer.data(), &reconstructed_byte, CHAR_BIT);
		sink_file.write(&reconstructed_byte, 1);
	}

	size_t main_chunk_size = (this->K - n_completing) / CHAR_BIT; // in byte
	this->n_left           = (this->K - n_completing) % CHAR_BIT;

	tools::Bit_packer::pack(V + n_completing, this->chunk.data(), main_chunk_size * CHAR_BIT);
	sink_file.write(this->chunk.data(), main_chunk_size);
	sink_file.flush();
	this->n_left = 0;
	for (size_t i = n_completing + main_chunk_size * CHAR_BIT; i < (size_t)this->K; i++)
		this->reconstructed_buffer[this->n_left++] = V[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Sink_user_binary<B_8>;
template class aff3ct::module::Sink_user_binary<B_16>;
template class aff3ct::module::Sink_user_binary<B_32>;
template class aff3ct::module::Sink_user_binary<B_64>;
#else
template class aff3ct::module::Sink_user_binary<B>;
#endif
// ==================================================================================== explicit template instantiation
