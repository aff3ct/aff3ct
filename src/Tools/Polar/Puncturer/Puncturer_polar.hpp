#ifndef PUNCTURER_POLAR_HPP_
#define PUNCTURER_POLAR_HPP_

#include "../../MIPP/mipp.h"

template <typename B, typename R>
class Puncturer_polar
{
public:
	Puncturer_polar() {};
	virtual ~Puncturer_polar() {};

	// classically, puncturers will generate their own frozen bits
	virtual void gen_frozen_bits(mipp::vector<B>& frozen_bits) = 0;
	virtual void puncture       (mipp::vector<R>& Y_N) const = 0;
};

#endif /* PUNCTURER_POLAR_HPP_ */