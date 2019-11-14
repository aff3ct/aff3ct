#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Codec/Codec_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename Q>
Codec_SISO<B,Q>
::Codec_SISO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec<B,Q>(K, N_cw, N, tail_length, n_frames), decoder_siso(nullptr)
{
}

template <typename B, typename Q>
module::Decoder_SISO<Q>& Codec_SISO<B,Q>
::get_decoder_siso()
{
	if (this->decoder_siso == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siso' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->decoder_siso;
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::set_decoder_siso(std::shared_ptr<module::Decoder_SISO<Q>> dec)
{
	this->decoder_siso = dec;
}

template <typename B, typename Q>
void Codec_SISO<B,Q>
::set_decoder_siso(module::Decoder_SISO<Q>* dec)
{
	this->set_decoder_siso(std::shared_ptr<module::Decoder_SISO<Q>>(dec));
}

}
}
