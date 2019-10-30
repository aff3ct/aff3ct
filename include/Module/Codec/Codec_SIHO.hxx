#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Codec_SIHO<B,Q>
::Codec_SIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_SIHO";
	this->set_name(name);
}

template <typename B, typename Q>
const std::shared_ptr<Decoder_SIHO<B,Q>>& Codec_SIHO<B,Q>
::get_decoder_siho()
{
	if (this->decoder_siho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siho' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->decoder_siho;
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::reset()
{
	if (this->decoder_siho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siho' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->decoder_siho->reset();
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::set_decoder_siho(std::shared_ptr<Decoder_SIHO<B,Q>> dec)
{
	this->decoder_siho = dec;
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::set_decoder_siho(Decoder_SIHO<B,Q>* dec)
{
	this->set_decoder_siho(std::shared_ptr<Decoder_SIHO<B,Q>>(dec));
}

}
}
