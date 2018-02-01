#ifndef DECODER_MAXIMUM_LIKELIHOOD_REDUCED_STD_HPP_
#define DECODER_MAXIMUM_LIKELIHOOD_REDUCED_STD_HPP_

#include "Decoder_maximum_likelihood_reduced_naive.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_maximum_likelihood_reduced_std : public Decoder_maximum_likelihood_reduced_naive<B,R>
{
protected:
	std::vector<uint32_t> flip_pos;

public:
	Decoder_maximum_likelihood_reduced_std(const int K, const int N, Encoder<B> &encoder, const uint32_t max_flips = 3, 
	                                       const bool hamming = false, const int n_frames = 1);
	virtual ~Decoder_maximum_likelihood_reduced_std();

protected:
	void _decode_siho   (const R *Y_N,  B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N, const int frame_id);
	void _decode_hiho   (const B *Y_N,  B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N,  B *V_N, const int frame_id);

	void _decode_siho_cw_recursive(const R *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips);
	void _decode_hiho_cw_recursive(const B *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips);
};

template <typename B = int, typename R = float>
using Decoder_ML_reduced_std = Decoder_maximum_likelihood_reduced_std<B,R>;
}
}

#endif /* DECODER_MAXIMUM_LIKELIHOOD_REDUCED_STD_HPP_ */
