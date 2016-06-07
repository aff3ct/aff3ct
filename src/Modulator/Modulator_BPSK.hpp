#ifndef MODULATOR_BPSK_HPP_
#define MODULATOR_BPSK_HPP_

#include <vector>
#include "../Tools/MIPP/mipp.h"

#include "Modulator.hpp"

template <typename B>
class Modulator_BPSK : public Modulator<B>
{
public:
	Modulator_BPSK();
	virtual ~Modulator_BPSK();

	virtual void modulate(mipp::vector<B>& X_N) const;
};

#endif /* MODULATOR_BPSK_HPP_ */
