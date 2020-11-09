/*!
 * \file
 * \brief Class module::Decoder_BCH_genius.
 */
#ifndef DECODER_BCH_GENIUS
#define DECODER_BCH_GENIUS

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

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
	Decoder_BCH_genius(const int K, const int N, const int t, Encoder<B> &encoder);
	virtual ~Decoder_BCH_genius() = default;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual int _decode        (      B *Y_N,         const size_t frame_id);
	virtual int _decode_hiho   (const B *Y_N, B *V_K, const size_t frame_id);
	virtual int _decode_hiho_cw(const B *Y_N, B *V_N, const size_t frame_id);
	virtual int _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	virtual int _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);
};
}
}

#endif /* DECODER_BCH_GENIUS */
