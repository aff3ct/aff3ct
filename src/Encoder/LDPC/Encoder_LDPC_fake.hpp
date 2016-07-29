#ifndef ENCODER_LDPC_FAKE_HPP_
#define ENCODER_LDPC_FAKE_HPP_

#include "../Encoder.hpp"

template <typename B>
class Encoder_LDPC_fake : public Encoder<B>
{
public:
	Encoder_LDPC_fake(const int n_frames = 1);
	virtual ~Encoder_LDPC_fake();

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#endif /* ENCODER_LDPC_FAKE_HPP_ */
