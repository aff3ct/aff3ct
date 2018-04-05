#ifndef CODEC_SISO_SIHO_HXX_
#define CODEC_SISO_SIHO_HXX_

#include "Codec_SISO_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename Q>
Codec_SISO_SIHO<B,Q>::
Codec_SISO_SIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_SISO<B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_SISO_SIHO";
	this->set_name(name);
}

template <typename B, typename Q>
Codec_SISO_SIHO<B,Q>::
~Codec_SISO_SIHO()
{
	Decoder_SISO<Q  > *siso = nullptr;
	Decoder_SIHO<B,Q> *siho = nullptr;

	try
	{
		siso = this->get_decoder_siso();
	} catch (const std::exception&)
	{
	}

	try
	{
		siho = this->get_decoder_siho();
	} catch (const std::exception&)
	{
	}

	if      (siso == nullptr && siho != nullptr) { delete siho; this->set_decoder_siho(nullptr); }
	else if (siho == nullptr && siso != nullptr) { delete siso; this->set_decoder_siso(nullptr); }
	else if (siho != nullptr && siso != nullptr)
	{
		// do not delete the siso if the siho and the siso are the same pointers
		if (dynamic_cast<module::Decoder*>(siso) != dynamic_cast<module::Decoder*>(siho))
			delete siso;
		delete siho;
		this->set_decoder_siho(nullptr);
		this->set_decoder_siso(nullptr);
	}
}

template <typename B, typename Q>
void Codec_SISO_SIHO<B,Q>::
reset()
{
	this->get_decoder_siho()->reset();
}

}
}

#endif /* CODEC_SISO_SIHO_HXX_ */
