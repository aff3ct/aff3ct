/*!
 * \file
 * \brief Class module::Decoder_RS_genius.
 */
#ifndef DECODER_RS_GENIUS
#define DECODER_RS_GENIUS

#include <vector>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/RS/Decoder_RS.hpp"

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
	Decoder_RS_genius(const int K, const int N, const tools::RS_polynomial_generator &GF, Encoder<B> &encoder);
	virtual ~Decoder_RS_genius() = default;

	virtual void set_n_frames(const size_t n_frames);

protected:
	virtual int _decode        (      S *Y_N,         const size_t frame_id);
	virtual int _decode_hiho   (const B *Y_N, B *V_K, const size_t frame_id);
	virtual int _decode_hiho_cw(const B *Y_N, B *V_N, const size_t frame_id);
	virtual int _decode_siho   (const R *Y_N, B *V_K, const size_t frame_id);
	virtual int _decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id);

private:
	int hamming_distance();
};
}
}

#endif /* DECODER_RS_GENIUS */
