#include "Tools/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>
::Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, n_frames),
  Codec_HIHO<B,Q>(K, N_cw, N, n_frames)
{
}

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>* Codec_SIHO_HIHO<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::deep_copy(const Codec_SIHO_HIHO<B,Q> &t)
{
	Codec<B,Q>::deep_copy(t);
	if (t.decoder_siho != nullptr) this->decoder_siho.reset(t.decoder_siho->clone());
	if (t.decoder_hiho != nullptr) this->decoder_hiho.reset(t.decoder_hiho->clone());
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::set_decoder_siho_hiho(std::shared_ptr<module::Decoder_SIHO_HIHO<B,Q>> dec)
{
	this->set_decoder_siho(dec);
	this->set_decoder_hiho(dec);
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::set_decoder_siho_hiho(module::Decoder_SIHO_HIHO<B,Q>* dec)
{
	this->set_decoder_siho_hiho(std::shared_ptr<module::Decoder_SIHO_HIHO<B,Q>>(dec));
}

}
}
