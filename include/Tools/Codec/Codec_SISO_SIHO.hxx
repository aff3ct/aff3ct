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
