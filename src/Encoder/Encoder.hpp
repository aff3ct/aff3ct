#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Encoder
{
public:
	Encoder() {};
	virtual ~Encoder() {};

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	virtual int tail_length() const { return 0; }
};

#endif /* ENCODER_HPP_ */
