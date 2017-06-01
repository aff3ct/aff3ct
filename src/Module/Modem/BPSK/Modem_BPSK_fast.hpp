#ifndef MODEM_BPSK_FAST_HPP_
#define MODEM_BPSK_FAST_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_BPSK_fast : public Modem<B,R,Q>
{
private:
	const bool disable_sig2;
	R two_on_square_sigma;
	
public:
	Modem_BPSK_fast(const int N, const R sigma = (R)1, const bool disable_sig2 = false, const int n_frames = 1,
	                const std::string name = "Modem_BPSK_fast");
	virtual ~Modem_BPSK_fast();

	void set_sigma(const R sigma);

	void   modulate(const B *X_N1,                R *X_N2); using Modem<B,R,Q>::modulate;
	void     filter(const R *Y_N1,                R *Y_N2); using Modem<B,R,Q>::filter;
	void demodulate(const Q *Y_N1,                Q *Y_N2);
	void demodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3); using Modem<B,R,Q>::demodulate;

	static int size_mod(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, false);
	}

	static int size_fil(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, false);
	}
};
}
}

#endif /* MODEM_BPSK_FAST_HPP_ */
