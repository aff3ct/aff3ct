#ifndef DECODER_BCH_GENIUS
#define DECODER_BCH_GENIUS

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "../Decoder_BCH.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH_genius : public Decoder_BCH<B,R>
{
protected:
	Encoder<B> &encoder;
	std::vector<B> YH_N; // hard decision input vector

public:
	Decoder_BCH_genius(const int K, const int N, const int t, Encoder<B> &encoder, const int n_frames = 1);
	virtual ~Decoder_BCH_genius() = default;

protected:
	virtual void _decode        (      B *Y_N,         const int frame_id);
	virtual void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	virtual void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
	virtual void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#endif /* DECODER_BCH_GENIUS */
