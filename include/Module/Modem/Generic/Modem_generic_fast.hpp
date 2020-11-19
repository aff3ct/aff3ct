/*!
 * \file
 * \brief Class module::Modem_generic_fast.
 */
#ifndef MODEM_GENERIC_FAST_HPP_
#define MODEM_GENERIC_FAST_HPP_

#include <memory>

#include "Tools/Math/max.h"
#include "Tools/Constellation/Constellation.hpp"
#include "Module/Modem/Generic/Modem_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, tools::proto_max  <Q> MAX  = tools::max_star,
                                                                tools::proto_max_i<Q> MAXI = tools::max_star_i>
class Modem_generic_fast : public Modem_generic<B,R,Q,MAX>
{
protected:
	std::vector<std::complex<Q>> cstl_bis;

public:
	Modem_generic_fast(const int N, const tools::Constellation<R> &cstl, const bool disable_sig2 = false);

	virtual ~Modem_generic_fast() = default;

	virtual Modem_generic_fast<B,R,Q,MAX,MAXI>* clone() const;

protected:
	void _demodulate_complex    (              const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	void _demodulate_wg_complex (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	void _demodulate_real       (              const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	void _demodulate_wg_real    (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);

	void _tdemodulate           (const float *CP,               const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	void _tdemodulate_wg        (const float *CP, const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/Generic/Modem_generic_fast.hxx"
#endif

#endif // MODEM_GENERIC_FAST_HPP_
