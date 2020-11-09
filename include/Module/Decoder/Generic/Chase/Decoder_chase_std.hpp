/*!
 * \file
 * \brief Class module::Decoder_chase_std.
 */
#ifndef DECODER_CHASE_STD_HPP_
#define DECODER_CHASE_STD_HPP_

#include <cstdint>
#include <vector>
#include <memory>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_chase_std : public Decoder_SIHO<B,R>
{
protected:
	std::shared_ptr<Encoder<B>> encoder;
	std::vector<B> best_X_N;
	std::vector<uint32_t> less_reliable_llrs;
	const uint32_t max_flips;
	const bool hamming;
	float min_euclidean_dist;
	uint32_t min_hamming_dist;
	uint32_t best_test;

public:
	Decoder_chase_std(const int K, const int N, const Encoder<B> &encoder, const uint32_t max_flips = 3,
	                  const bool hamming = false);
	virtual ~Decoder_chase_std() = default;
	virtual Decoder_chase_std<B,R>* clone() const;

protected:
	virtual void deep_copy(const Decoder_chase_std<B,R> &m);
	int _decode_siho   (const R *Y_N,  B *V_K, const size_t frame_id);
	int _decode_siho_cw(const R *Y_N,  B *V_N, const size_t frame_id);

	void _decode_siho_cw_euclidean(const R *Y_N, B *V_N);
	void _decode_siho_cw_hamming  (              B *V_N);
};
}
}

#endif /* DECODER_CHASE_STD_HPP_ */
