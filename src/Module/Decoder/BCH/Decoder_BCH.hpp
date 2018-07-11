#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

#include "../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH : public Decoder_SIHO_HIHO<B,R>
{
protected:
	const int t;         // correction power
	const int N_p2;      // the next power 2 of N minus 1
	std::vector<B> YH_N; // hard decision input vector

public:
	bool last_is_codeword;

	Decoder_BCH(const int K, const int N, const int t, const int n_frames = 1);

	virtual ~Decoder_BCH();

protected:
	virtual void _decode        (      B *Y_N,         const int frame_id) = 0;
	virtual void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	virtual void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
	virtual void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	virtual void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#endif /* DECODER_BCH */
