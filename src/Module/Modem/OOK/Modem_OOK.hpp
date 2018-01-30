#ifndef MODEM_OOK_HPP_
#define MODEM_OOK_HPP_

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK : public Modem<B,R,Q>
{
protected:
	const bool disable_sig2;
	R sigma_factor;

public:
	Modem_OOK(const int N, const R sigma = (R)1, const bool disable_sig2 = false, const int n_frames = 1);
	virtual ~Modem_OOK();

	void set_sigma(const R sigma);

	static int size_mod(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, false);
	}

	static int size_fil(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, false);
	}

protected:
	void   _modulate    (              const B *X_N1,                R *X_N2, const int frame_id);
	void     _filter    (              const R *Y_N1,                R *Y_N2, const int frame_id);
	void _demodulate    (              const Q *Y_N1,                Q *Y_N2, const int frame_id);
	void _demodulate_wg (const R *H_N, const Q *Y_N1,                Q *Y_N2, const int frame_id);
	void _tdemodulate   (              const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id);
};
}
}

#endif /* MODEM_OOK_HPP_ */
