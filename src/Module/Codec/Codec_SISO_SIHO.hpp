#ifndef CODEC_SISO_SIHO_HPP_
#define CODEC_SISO_SIHO_HPP_

#include "Module/Decoder/Decoder_SISO.hpp"

#include "Codec_SISO.hpp"
#include "Codec_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SISO_SIHO : public Codec_SISO<B,Q>, public Codec_SIHO<B,Q>
{
public:
	Codec_SISO_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1,
	                const std::string name = "Codec_SISO_SIHO")
	: Codec_SISO<B,Q>(K, N_cw, N, tail_length, n_frames, name          ),
	  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames, name + "_siho")
	{
	}

	virtual ~Codec_SISO_SIHO()
	{
	}

	tools::Interleaver_core<>* get_interleaver()
	{
		return Codec_SISO<B,Q>::get_interleaver();
	}

	Encoder<B>* get_encoder()
	{
		return Codec_SISO<B,Q>::get_encoder();
	}

	Puncturer<B,Q>* get_puncturer()
	{
		return Codec_SISO<B,Q>::get_puncturer();
	}

	float get_sigma()
	{
		return Codec_SISO<B,Q>::get_sigma();
	}

	virtual void set_sigma(const float sigma)
	{
		Codec_SISO<B,Q>::set_sigma(sigma);
	}

protected:
	void set_interleaver(tools::Interleaver_core<>* itl)
	{
		Codec_SISO<B,Q>::set_interleaver(itl);
	}

	void set_encoder(Encoder<B>* enc)
	{
		Codec_SISO<B,Q>::set_encoder(enc);
	}

	void set_puncturer(Puncturer<B,Q>* pct)
	{
		Codec_SISO<B,Q>::set_puncturer(pct);
	}
};
}
}

#endif /* CODEC_SISO_SIHO_HPP_ */
