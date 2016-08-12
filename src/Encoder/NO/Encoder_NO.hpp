#ifndef ENCODER_NO_HPP_
#define ENCODER_NO_HPP_

#include "../Encoder.hpp"

template <typename B>
class Encoder_NO : public Encoder<B>
{
public:
	Encoder_NO(const int n_frames = 1);
	virtual ~Encoder_NO();

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

#endif /* ENCODER_NO_HPP_ */
