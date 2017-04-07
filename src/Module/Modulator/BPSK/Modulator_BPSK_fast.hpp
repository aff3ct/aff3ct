#ifndef MODULATOR_BPSK_FAST_HPP_
#define MODULATOR_BPSK_FAST_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Modulator.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modulator_BPSK_fast : public Modulator<B,R,Q>
{
private:
	const bool disable_sig2;
	const R two_on_square_sigma;
	
public:
	Modulator_BPSK_fast(const int N, const R sigma, const bool disable_sig2 = false, const int n_frames = 1,
	                    const std::string name = "Modulator_BPSK_fast");
	virtual ~Modulator_BPSK_fast();

	void   modulate(const B *X_N1,                R *X_N2); using Modulator<B,R,Q>::modulate;
	void demodulate(const Q *Y_N1,                Q *Y_N2);
	void demodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3); using Modulator<B,R,Q>::demodulate;

	static int size_mod(const int N)
	{
		return Modulator<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, false);
	}

	static int size_fil(const int N)
	{
		return Modulator<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, false);
	}
};
}
}

#endif /* MODULATOR_BPSK_FAST_HPP_ */
