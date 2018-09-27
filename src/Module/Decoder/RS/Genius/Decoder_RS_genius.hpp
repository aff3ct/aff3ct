#ifndef DECODER_RS_GENIUS
#define DECODER_RS_GENIUS

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "../Decoder_RS.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RS_genius : public Decoder_RS<B,R>
{
public:
	using typename Decoder_RS<B,R>::S; // symbol to represent data

protected:
	Encoder<B> &encoder;
	std::vector<S> X_Ns; // packed as symbols X_N

public:
	Decoder_RS_genius(const int K, const int N, const tools::RS_polynomial_generator &GF, Encoder<B> &encoder, const int n_frames = 1);
	virtual ~Decoder_RS_genius() = default;

protected:
	virtual void _decode        (      S *Y_N,         const int frame_id);
	virtual void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	virtual void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
	virtual void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);

private:
	int hamming_distance();
};
}
}

#endif /* DECODER_RS_GENIUS */
