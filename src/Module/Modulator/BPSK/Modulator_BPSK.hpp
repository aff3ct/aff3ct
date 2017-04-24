#ifndef MODULATOR_BPSK_HPP_
#define MODULATOR_BPSK_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Modulator.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modulator_BPSK : public Modulator<B,R,Q>
{
private:
	const bool disable_sig2;
	const R two_on_square_sigma;

public:
	Modulator_BPSK(const int N, const R sigma, const bool disable_sig2 = false, const int n_frames = 1,
	               const std::string name = "Modulator_BPSK");
	virtual ~Modulator_BPSK();

	void   modulate           (const B *X_N1,                              R *X_N2); using Modulator<B,R,Q>::modulate;
	void     filter           (const R *Y_N1,                              R *Y_N2); using Modulator<B,R,Q>::filter;
	void demodulate           (const Q *Y_N1,                              Q *Y_N2);
	void demodulate_with_gains(const Q *Y_N1, const R *H_N,                Q *Y_N2);
	void demodulate           (const Q *Y_N1,               const Q *Y_N2, Q *Y_N3); using Modulator<B,R,Q>::demodulate;
	void demodulate_with_gains(const Q *Y_N1, const R *H_N, const Q *Y_N2, Q *Y_N3); using Modulator<B,R,Q>::demodulate_with_gains;

	static int size_mod(const int N)
	{
		return Modulator<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, false);
	}

	static int size_fil(const int N)
	{
		return Modulator<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, false);
	}

	// void set_sigma(const R sigma)
	// {
	// 	this->sigma = sigma;
	// 	this->two_on_square_sigma = (R)2.0 / (sigma * sigma);
	// }

};
}
}

#endif /* MODULATOR_BPSK_HPP_ */
