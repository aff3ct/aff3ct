#ifndef CODEC_SIHO_HIHO_HXX_
#define CODEC_SIHO_HIHO_HXX_

#include "Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>::
Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_HIHO<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_SIHO_HIHO";
	this->set_name(name);
}

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>::
~Codec_SIHO_HIHO()
{
	Decoder_HIHO<B  > *hiho = nullptr;
	Decoder_SIHO<B,Q> *siho = nullptr;

	try
	{
		siho = this->get_decoder_siho();
	} catch (const std::exception&)
	{
	}

	try
	{
		hiho = this->get_decoder_hiho();
	} catch (const std::exception&)
	{
	}

	if      (siho == nullptr && hiho != nullptr) { delete hiho; this->set_decoder_hiho(nullptr); }
	else if (hiho == nullptr && siho != nullptr) { delete siho; this->set_decoder_siho(nullptr); }
	else if (siho != nullptr && hiho != nullptr)
	{
		// do not delete the hiho if the siho and the hiho are the same pointers
		if (dynamic_cast<module::Decoder*>(hiho) != dynamic_cast<module::Decoder*>(siho))
			delete hiho;
		delete siho;
		this->set_decoder_siho(nullptr);
		this->set_decoder_hiho(nullptr);
	}
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>::
reset()
{
	this->get_decoder_siho()->reset();
}

}
}

#endif /* CODEC_SIHO_HIHO_HXX_ */
