#ifndef MODEM_PAM_HPP_
#define MODEM_PAM_HPP_

#include <vector>

#include "Tools/Math/max.h"

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX = tools::max_star>
class Modem_PAM : public Modem<B,R,Q>
{
private:
	const int bits_per_symbol;
	const int nbr_symbols;
	const R sqrt_es;
	const bool disable_sig2;
	std::vector<R> constellation;
	R inv_sigma2;

public:
	Modem_PAM(const int N, const tools::Noise<R>& noise = tools::Noise<R>(),
	          const int bits_per_symbol = 1, const bool disable_sig2 = false,
	          const int n_frames = 1);
	virtual ~Modem_PAM();

	virtual void set_noise(const tools::Noise<R>& noise);

	static bool is_complex_mod()
	{
		return false;
	}

	static bool is_complex_fil()
	{
		return false;
	}

	static int size_mod(const int N, const int bps)
	{
		return Modem<B,R,Q>::get_buffer_size_after_modulation(N, bps, 0, 1, is_complex_mod());
	}

	static int size_fil(const int N, const int bps)
	{
		return Modem<B,R,Q>::get_buffer_size_after_filtering(N, bps, 0, 1, is_complex_fil());
	}

protected:
	void   _tmodulate   (              const Q *X_N1,                 R *X_N2, const int frame_id);
	void   _modulate    (              const B *X_N1,                 R *X_N2, const int frame_id);
	void     _filter    (              const R *Y_N1,                 R *Y_N2, const int frame_id);
	void _demodulate    (              const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _demodulate_wg (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _tdemodulate   (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg(const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);

private:
	inline R bits_to_symbol(const B* bits) const;
};
}
}

#include "Modem_PAM.hxx"

#endif // MODEM_PAM_HPP_
