#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Codec/Codec_SIHO.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
Codec_SIHO<B,Q>
::Codec_SIHO(const int K, const int N_cw, const int N, const int n_frames)
: Codec<B,Q>(K, N_cw, N, n_frames)
{
}

template <typename B, typename Q>
Codec_SIHO<B,Q>* Codec_SIHO<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::deep_copy(const Codec_SIHO<B,Q> &t)
{
	Codec<B,Q>::deep_copy(t);
	if (t.decoder_siho != nullptr) this->decoder_siho.reset(t.decoder_siho->clone());
}

template <typename B, typename Q>
module::Decoder_SIHO<B,Q>& Codec_SIHO<B,Q>
::get_decoder_siho()
{
	if (this->decoder_siho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_siho' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->decoder_siho;
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::set_decoder_siho(std::shared_ptr<module::Decoder_SIHO<B,Q>> dec)
{
	this->decoder_siho = dec;
}

template <typename B, typename Q>
void Codec_SIHO<B,Q>
::set_decoder_siho(module::Decoder_SIHO<B,Q>* dec)
{
	this->set_decoder_siho(std::shared_ptr<module::Decoder_SIHO<B,Q>>(dec));
}

}
}
