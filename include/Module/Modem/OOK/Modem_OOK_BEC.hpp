/*!
 * \file
 * \brief Class module::Modem_OOK_BEC.
 */
#ifndef MODEM_OOK_BEC_HPP_
#define MODEM_OOK_BEC_HPP_

#include "Module/Modem/OOK/Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK_BEC : public Modem_OOK<B,R,Q>
{
public:
	Modem_OOK_BEC(const int N);
	virtual ~Modem_OOK_BEC() = default;

	virtual Modem_OOK_BEC<B,R,Q>* clone() const;

protected:
	void _demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id);
};
}
}

#endif /* MODEM_OOK_BEC_HPP_ */
