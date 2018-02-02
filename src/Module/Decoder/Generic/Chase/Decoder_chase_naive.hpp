#ifndef DECODER_CHASE_NAIVE_HPP_
#define DECODER_CHASE_NAIVE_HPP_

#include "Module/Encoder/Encoder.hpp"

#include "../../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_chase_naive : public Decoder_SIHO<B,R>
{
protected:
	Encoder<B> &encoder;

	std::vector<B> X_N;
	std::vector<B> best_X_N;
	std::vector<B> hard_Y_N;

	const uint32_t max_flips;
	const bool hamming;
	float min_euclidean_dist;
	uint32_t min_hamming_dist;

	std::vector<uint32_t> llrs_indexes;
	std::vector<uint32_t> flip_pos;

public:
	Decoder_chase_naive(const int K, const int N, Encoder<B> &encoder, const uint32_t max_flips = 3, 
	                    const bool hamming = false, const int n_frames = 1);
	virtual ~Decoder_chase_naive();

protected:
	void _decode_siho   (const R *Y_N,  B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N, const int frame_id);

	void _decode_siho_cw_recursive     (const R *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips);
	bool _decode_siho_cw_recursive_hard(const B *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips);
};
}
}

#endif /* DECODER_CHASE_NAIVE_HPP_ */
