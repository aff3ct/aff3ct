/*!
 * \file
 * \brief Class module::Modem_OOK_optical_rop_estimate.
 */
#ifndef MODEM_OOK_OPTICAL_ROP_ESTIMATE_HPP_
#define MODEM_OOK_OPTICAL_ROP_ESTIMATE_HPP_

#include <cstddef>
#include <vector>
#include <memory>

#include "Tools/Noise/Received_optical_power.hpp"
#include "Tools/Math/Distribution/Distributions.hpp"
#include "Module/Modem/OOK/Modem_OOK.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK_optical_rop_estimate : public Modem_OOK<B,R,Q>
{
protected:
	std::vector<size_t> ROP_known_bits;
	const tools::Distributions<R>& dist;

	std::vector<R> dist_ROP_range;
	std::vector<R> dist_meanvoltage_p0;
	std::vector<R> dist_meanvoltage_p1;

public:
	/*
	 * The 'ROP_known_bits' positions of the original frame are used for the ROP estimation
	 */
	Modem_OOK_optical_rop_estimate(const int N, const std::vector<size_t> &ROP_known_bits,
	                               const tools::Distributions<R>& dist);

	/*
	 * The 'n_known_bits' first bits are supposed known for the ROP estimation
	 */
	Modem_OOK_optical_rop_estimate(const int N, int n_known_bits,
	                               const tools::Distributions<R>& dist);
	virtual ~Modem_OOK_optical_rop_estimate() = default;

	virtual Modem_OOK_optical_rop_estimate<B,R,Q>* clone() const;

protected:
	// H_N is replaced by X_N, that is the output of the modulation in order to estimate values
	void _demodulate_wg(const float *CP, const R *X_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id);

private:
	void init();
};
}
}
#endif /* MODEM_OOK_OPTICAL_ROP_ESTIMATE_HPP_ */
