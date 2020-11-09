#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Codec/Codec_HIHO.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
Codec_HIHO<B,Q>
::Codec_HIHO(const int K, const int N_cw, const int N)
: Codec<B,Q>(K, N_cw, N)
{
}

template <typename B, typename Q>
Codec_HIHO<B,Q>* Codec_HIHO<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>
::deep_copy(const Codec_HIHO<B,Q> &t)
{
	Codec<B,Q>::deep_copy(t);
	if (t.decoder_hiho != nullptr) this->decoder_hiho.reset(t.decoder_hiho->clone());
}

template <typename B, typename Q>
module::Decoder_HIHO<B>& Codec_HIHO<B,Q>
::get_decoder_hiho()
{
	if (this->decoder_hiho == nullptr)
	{
		std::stringstream message;
		message << "'decoder_hiho' is NULL.";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return *this->decoder_hiho;
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>
::set_decoder_hiho(module::Decoder_HIHO<B>* dec)
{
	this->set_decoder_hiho(std::shared_ptr<module::Decoder_HIHO<B>>(dec));
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>
::set_decoder_hiho(std::shared_ptr<module::Decoder_HIHO<B>> dec)
{
	this->decoder_hiho = dec;
}

template <typename B, typename Q>
void Codec_HIHO<B,Q>
::set_n_frames(const size_t n_frames)
{
	Codec<B,Q>::set_n_frames(n_frames);
	if (this->decoder_hiho != nullptr)
		this->decoder_hiho->set_n_frames(n_frames);
}

}
}
