#ifndef MODEM_OOK_BEC_HPP_
#define MODEM_OOK_BEC_HPP_

#include "Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK_BEC : public Modem_OOK<B,R,Q>
{
public:
	Modem_OOK_BEC(const int N, const tools::Noise<R>& noise = tools::EP<R>(), const int n_frames = 1);
	virtual ~Modem_OOK_BEC() = default;

protected:
	void _demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id);
	void check_noise();
};
}
}

#endif /* MODEM_OOK_BEC_HPP_ */
