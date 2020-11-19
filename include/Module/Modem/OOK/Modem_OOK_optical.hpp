/*!
 * \file
 * \brief Class module::Modem_OOK_optical.
 */
#ifndef MODEM_OOK_OPTICAL_HPP_
#define MODEM_OOK_OPTICAL_HPP_

#include "Tools/Math/Distribution/Distributions.hpp"
#include "Module/Modem/OOK/Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK_optical : public Modem_OOK<B,R,Q>
{
protected:
	const tools::Distributions<R>& dist;
	const tools::Distribution<R>* current_dist;

public:
	Modem_OOK_optical(const int N, const tools::Distributions<R>& dist);
	virtual ~Modem_OOK_optical() = default;

	virtual Modem_OOK_optical<B,R,Q>* clone() const;

protected:
	void _demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id);
};
}
}

#endif /* MODEM_OOK_OPTICAL_HPP_ */
