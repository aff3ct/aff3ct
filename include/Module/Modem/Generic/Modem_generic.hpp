/*!
 * \file
 * \brief Class module::Modem_generic.
 */
#ifndef MODEM_GENERIC_HPP_
#define MODEM_GENERIC_HPP_

#include <memory>

#include "Tools/Math/max.h"
#include "Tools/Constellation/Constellation.hpp"
#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX = tools::max_star>
class Modem_generic : public Modem<B,R,Q>
{
protected:
	const tools::Constellation<R> &cstl;

	const int bits_per_symbol;
	const int nbr_symbols;
	const bool disable_sig2;
	R inv_sigma2;

public:
	Modem_generic(const int N, const tools::Constellation<R> &cstl, const bool disable_sig2 = false);

	virtual ~Modem_generic() = default;

	virtual Modem_generic<B,R,Q,MAX>* clone() const;

	static bool is_complex_mod(const tools::Constellation<R>& c);
	static bool is_complex_fil(const tools::Constellation<R>& c);
	static int size_mod(const int N, const tools::Constellation<R>& c);
	static int size_fil(const int N, const tools::Constellation<R>& c);

protected:
	virtual void   _tmodulate   (                               const Q *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void   _modulate    (                               const B *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void     _filter    (const float *CP,               const R *Y_N1,                 R *Y_N2, const size_t frame_id);
	virtual void _demodulate    (const float *CP,               const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _demodulate_wg (const float *CP, const R *H_N, const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _tdemodulate   (const float *CP,               const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	virtual void _tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);

	virtual void   _tmodulate_complex   (              const Q *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void   _tmodulate_real      (              const Q *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void   _modulate_complex    (              const B *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void   _modulate_real       (              const B *X_N1,                 R *X_N2, const size_t frame_id);
	virtual void _demodulate_complex    (              const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _demodulate_real       (              const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _demodulate_wg_complex (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _demodulate_wg_real    (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const size_t frame_id);
	virtual void _tdemodulate_complex   (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	virtual void _tdemodulate_real      (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	virtual void _tdemodulate_wg_complex(const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	virtual void _tdemodulate_wg_real   (const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const size_t frame_id);

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/Generic/Modem_generic.hxx"
#endif

#endif // MODEM_GENERIC_HPP_
