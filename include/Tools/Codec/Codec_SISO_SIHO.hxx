#include "Tools/Codec/Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B, typename Q>
Codec_SISO_SIHO<B,Q>
::Codec_SISO_SIHO(const int K, const int N_cw, const int N, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, n_frames),
  Codec_SISO<B,Q>(K, N_cw, N, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, n_frames)
{
}

template <typename B, typename Q>
Codec_SISO_SIHO<B,Q>* Codec_SISO_SIHO<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec_SISO_SIHO<B,Q>
::deep_copy(const Codec_SISO_SIHO<B,Q> &t)
{
	Codec<B,Q>::deep_copy(t);
	if (t.decoder_siso != nullptr) this->decoder_siso.reset(t.decoder_siso->clone());
	if (t.decoder_siho != nullptr) this->decoder_siho.reset(t.decoder_siho->clone());
}

template <typename B, typename Q>
void Codec_SISO_SIHO<B,Q>
::set_decoder_siso_siho(std::shared_ptr<module::Decoder_SISO_SIHO<B,Q>> dec)
{
	this->set_decoder_siso(dec);
	this->set_decoder_siho(dec);
}

template <typename B, typename Q>
void Codec_SISO_SIHO<B,Q>
::set_decoder_siso_siho(module::Decoder_SISO_SIHO<B,Q>* dec)
{
	this->set_decoder_siso_siho(std::shared_ptr<module::Decoder_SISO_SIHO<B,Q>>(dec));
}

}
}
