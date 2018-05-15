#ifndef MODEM_OOK_AWGN_HPP_
#define MODEM_OOK_AWGN_HPP_

#include "Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK_AWGN : public Modem_OOK<B,R,Q>
{
protected:
	const bool disable_sig2;
	R sigma_factor;

public:
	Modem_OOK_AWGN(const int N, const tools::Noise<R>& noise = tools::Sigma<R>(), const bool disable_sig2 = false, const int n_frames = 1);
	virtual ~Modem_OOK_AWGN() = default;

	virtual void set_noise(const tools::Noise<R>& noise);

protected:
	void _demodulate    (              const Q *Y_N1,                Q *Y_N2, const int frame_id);
	void _demodulate_wg (const R *H_N, const Q *Y_N1,                Q *Y_N2, const int frame_id);
	void _tdemodulate   (              const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id);

	void check_noise();
};
}
}

#endif /* MODEM_OOK_AWGN_HPP_ */
