#ifndef MODULATOR_HPP_
#define MODULATOR_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Modulator
{
public:
	Modulator() {};
	virtual ~Modulator() {};

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const = 0;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const = 0;

	virtual int get_buffer_size(const int N) { return N; }
};

#endif /* MODULATOR_HPP_ */
