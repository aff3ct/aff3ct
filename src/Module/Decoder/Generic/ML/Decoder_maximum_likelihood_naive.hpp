#ifndef DECODER_MAXIMUM_LIKELIHOOD_NAIVE_HPP_
#define DECODER_MAXIMUM_LIKELIHOOD_NAIVE_HPP_

#include "Decoder_maximum_likelihood.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_maximum_likelihood_naive : public Decoder_maximum_likelihood<B,R>
{
protected:
	const bool hamming;
	uint64_t x_max;
	float min_euclidean_dist;
	uint32_t min_hamming_dist;

public:
	Decoder_maximum_likelihood_naive(const int K, const int N, Encoder<B> &encoder, const bool hamming = false,
	                                 const int n_frames = 1);
	virtual ~Decoder_maximum_likelihood_naive() = default;

protected:
	void _decode_siho   (const R *Y_N,  B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N, const int frame_id);
	void _decode_hiho   (const B *Y_N,  B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N,  B *V_N, const int frame_id);
};

template <typename B = int, typename R = float>
using Decoder_ML_naive = Decoder_maximum_likelihood_naive<B,R>;
}
}

#endif /* DECODER_MAXIMUM_LIKELIHOOD_NAIVE_HPP_ */
