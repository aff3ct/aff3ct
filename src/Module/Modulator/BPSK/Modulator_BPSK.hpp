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

	void   modulate           (const B *X_N1,                     R *X_N2);
	void demodulate           (const Q *Y_N1,                     Q *Y_N2);
	void demodulate_with_gains(const Q *Y_N1, const R *H_N,       Q *Y_N2);
	void demodulate           (const Q *Y_N1,               const Q *Y_N2, Q *Y_N3);
	void demodulate_with_gains(const Q *Y_N1, const R *H_N, const Q *Y_N2, Q *Y_N3);

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

#endif /* MODULATOR_BPSK_HPP_ */
