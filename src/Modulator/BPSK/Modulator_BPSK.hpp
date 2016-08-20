#ifndef MODULATOR_BPSK_HPP_
#define MODULATOR_BPSK_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Modulator.hpp"

template <typename B, typename R, typename Q>
class Modulator_BPSK : public Modulator<B,R,Q>
{
private:
	const bool disable_sig2;
	const R two_on_square_sigma;

public:
	Modulator_BPSK(const int N, const R sigma, const int n_frames = 1, const std::string name = "Modulator_BPSK");
	virtual ~Modulator_BPSK();

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);
};

#endif /* MODULATOR_BPSK_HPP_ */
