#ifndef CODEC_SIHO_HPP_
#define CODEC_SIHO_HPP_

#include "Module/Decoder/Decoder_SIHO.hpp"

#include "Codec.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec_SIHO : virtual public Codec<B,Q>
{
private:
	Decoder_SIHO<B,Q>* decoder_siho;

public:
	Codec_SIHO(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1);

	virtual ~Codec_SIHO();

	Decoder_SIHO<B,Q>* get_decoder_siho();

	virtual void reset();

protected:
	void set_decoder_siho(Decoder_SIHO<B,Q>* dec);
};
}
}
#include "Codec_SIHO.hxx"

#endif /* CODEC_SIHO_HPP_ */
