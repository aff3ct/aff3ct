#ifndef MODULATOR_BPSK_FAST_HPP_
#define MODULATOR_BPSK_FAST_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Modulator.hpp"

template <typename B, typename R, typename Q>
class Modulator_BPSK_fast : public Modulator<B,R,Q>
{
private:
	const bool disable_sig2;
	const R two_on_square_sigma;
	
public:
	Modulator_BPSK_fast(const int N, const R sigma, const bool disable_sig2 = false, const int n_frames = 1, 
	                    const std::string name = "Modulator_BPSK_fast");
	virtual ~Modulator_BPSK_fast();

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);
};

#endif /* MODULATOR_BPSK_FAST_HPP_ */
