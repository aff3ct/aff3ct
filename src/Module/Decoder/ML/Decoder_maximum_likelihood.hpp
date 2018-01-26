#ifndef DECODER_MAXIMUM_LIKELIHOOD_HPP_
#define DECODER_MAXIMUM_LIKELIHOOD_HPP_

#include "Module/Encoder/Encoder.hpp"

#include "../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_maximum_likelihood : public Decoder_SIHO_HIHO<B,R>
{
protected:
	Encoder<B> &encoder;
	const bool hamming;
	std::vector<B> U_K;
	std::vector<B> X_N;
	std::vector<B> best_U_K;
	std::vector<B> best_X_N;
	std::vector<B> hard_Y_N;
	uint64_t u_max;
	float min_euclidean_dist;
	uint64_t min_hamming_dist;

public:
	Decoder_maximum_likelihood(const int K, const int N, Encoder<B> &encoder, const bool hamming = false,
	                           const int n_frames = 1);
	virtual ~Decoder_maximum_likelihood();

protected:
	void _decode_siho   (const R *Y_N,  B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N, const int frame_id);
	void _decode_hiho   (const B *Y_N,  B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N,  B *V_N, const int frame_id);
};

template <typename B = int, typename R = float>
using Decoder_ML = Decoder_maximum_likelihood<B,R>;
}
}

#endif /* DECODER_MAXIMUM_LIKELIHOOD_HPP_ */
