/*!
 * \file
 * \brief Class module::Modem_OOK_AWGN.
 */
#ifndef MODEM_OOK_AWGN_HPP_
#define MODEM_OOK_AWGN_HPP_

#include "Module/Modem/OOK/Modem_OOK.hpp"

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
	Modem_OOK_AWGN(const int N,
	               const bool disable_sig2 = false);
	virtual ~Modem_OOK_AWGN() = default;

	virtual Modem_OOK_AWGN<B,R,Q>* clone() const;

protected:
	void _demodulate    (const float *CP,               const Q *Y_N1,                Q *Y_N2, const size_t frame_id);
	void _demodulate_wg (const float *CP, const R *H_N, const Q *Y_N1,                Q *Y_N2, const size_t frame_id);
	void _tdemodulate   (const float *CP,               const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	void _tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);
};
}
}

#endif /* MODEM_OOK_AWGN_HPP_ */
