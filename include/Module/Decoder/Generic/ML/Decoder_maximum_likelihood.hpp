#ifndef DECODER_MAXIMUM_LIKELIHOO_HPP_
#define DECODER_MAXIMUM_LIKELIHOO_HPP_

#include <vector>

#include "Module/Encoder/Encoder.hpp"
#include "Module/Decoder/Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_maximum_likelihood : public Decoder_SIHO_HIHO<B,R>
{
protected:
	Encoder<B> &encoder;
	std::vector<B> U_K;
	std::vector<B> X_N;
	std::vector<B> best_U_K;
	std::vector<B> best_X_N;
	std::vector<B> hard_Y_N;

public:
	virtual ~Decoder_maximum_likelihood() = default;

protected:
	Decoder_maximum_likelihood(const int K, const int N, Encoder<B> &encoder, const int n_frames = 1);

	inline float    compute_euclidean_dist(const B *X_N, const R *Y_N) const;
	inline uint32_t compute_hamming_dist  (const B *X_N, const B *Y_N) const;
};
}
}

#include "Module/Decoder/Generic/ML/Decoder_maximum_likelihood.hxx"

#endif /* DECODER_MAXIMUM_LIKELIHOO_HPP_ */
