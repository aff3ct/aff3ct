#ifndef DECODER_GENERIC_HPP_
#define DECODER_GENERIC_HPP_

#include "Module/Encoder/Encoder.hpp"

#include "../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_generic : public Decoder_SIHO_HIHO<B,R>
{
protected:
	Encoder<B> &encoder;
	std::vector<B> U_K;
	std::vector<B> X_N;
	std::vector<B> best_U_K;
	std::vector<B> best_X_N;
	std::vector<B> hard_Y_N;

public:
	virtual ~Decoder_generic();

protected:
	Decoder_generic(const int K, const int N, Encoder<B> &encoder, const int n_frames = 1);

	inline float    compute_euclidean_dist     (const B *X_N,  const R *Y_N) const;
	inline float    compute_euclidean_dist_soft(const R *Y_N1, const R *Y_N2) const;
	inline uint32_t compute_hamming_dist       (const B *X_N,  const B *Y_N) const;
	inline uint32_t compute_hamming_dist_soft  (const B *X_N,  const R *Y_N) const;
};
}
}

#include "Decoder_generic.hxx"

#endif /* DECODER_GENERIC_HPP_ */
