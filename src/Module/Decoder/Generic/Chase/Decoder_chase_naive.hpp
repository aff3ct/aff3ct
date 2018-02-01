#ifndef DECODER_CHASE_NAIVE_HPP_
#define DECODER_CHASE_NAIVE_HPP_

#include "../Decoder_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_chase_naive : public Decoder_generic<B,R>
{
protected:
	const uint32_t max_flips;
	const bool hamming;
	uint64_t x_max;
	float min_euclidean_dist;
	uint32_t min_hamming_dist;

	std::vector<uint32_t> llrs_indexes;
	std::vector<uint32_t> flip_pos;

public:
	Decoder_chase_naive(const int K, const int N, Encoder<B> &encoder, const uint32_t max_flips = 3, 
	                    const bool hamming = false,
	                    const int n_frames = 1);
	virtual ~Decoder_chase_naive();

protected:
	void _decode_siho   (const R *Y_N,  B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N,  B *V_N, const int frame_id);
	void _decode_hiho   (const B *Y_N,  B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N,  B *V_N, const int frame_id);

	void _decode_siho_cw_recursive(const R *Y_N, B *V_N, const uint32_t n_flips, const uint32_t max_flips);
};
}
}

#endif /* DECODER_CHASE_NAIVE_HPP_ */
