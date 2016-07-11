#ifndef MODULATOR_BPSK_FAST_HPP_
#define MODULATOR_BPSK_FAST_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Modulator.hpp"

template <typename B, typename R>
class Modulator_BPSK_fast : public Modulator<B,R>
{
public:
	Modulator_BPSK_fast();
	virtual ~Modulator_BPSK_fast();

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const;
};

#endif /* MODULATOR_BPSK_FAST_HPP_ */
