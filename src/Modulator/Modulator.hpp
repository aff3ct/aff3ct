#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B>
class Modulator
{
public:
	Modulator() {};
	virtual ~Modulator() {};

	virtual void modulate(mipp::vector<B>& X_N) const = 0;
};

#endif /* MODULATOR_HPP_ */
