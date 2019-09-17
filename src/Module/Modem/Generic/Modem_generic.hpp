#ifndef MODEM_GENERIC_HPP_
#define MODEM_GENERIC_HPP_

#include <memory>

#include "Tools/Math/max.h"
#include "Tools/Constellation/Constellation.hpp"
#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, tools::proto_max<Q> MAX = tools::max_star>
class Modem_generic : public Modem<B,R,Q>
{
private:
	std::unique_ptr<const tools::Constellation<R>> cstl;

	const int bits_per_symbol;
	const int nbr_symbols;
	const bool disable_sig2;
	R inv_sigma2;

public:
	Modem_generic(const int N, std::unique_ptr<const tools::Constellation<R>>&& cstl, const tools::Noise<R>& noise = tools::Sigma<R>(),
	              const bool disable_sig2 = false, const int n_frames = 1);

	virtual ~Modem_generic() = default;

	virtual void set_noise(const tools::Noise<R>& noise);

	static bool is_complex_mod(const tools::Constellation<R>& c);
	static bool is_complex_fil(const tools::Constellation<R>& c);
	static int size_mod(const int N, const tools::Constellation<R>& c);
	static int size_fil(const int N, const tools::Constellation<R>& c);

protected:
	void   _tmodulate   (              const Q *X_N1,                 R *X_N2, const int frame_id);
	void   _modulate    (              const B *X_N1,                 R *X_N2, const int frame_id);
	void     _filter    (              const R *Y_N1,                 R *Y_N2, const int frame_id);
	void _demodulate    (              const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _demodulate_wg (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _tdemodulate   (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg(const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);

	void   _tmodulate_complex   (              const Q *X_N1,                 R *X_N2, const int frame_id);
	void   _tmodulate_real      (              const Q *X_N1,                 R *X_N2, const int frame_id);
	void   _modulate_complex    (              const B *X_N1,                 R *X_N2, const int frame_id);
	void   _modulate_real       (              const B *X_N1,                 R *X_N2, const int frame_id);
	void _demodulate_complex    (              const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _demodulate_real       (              const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _demodulate_wg_complex (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _demodulate_wg_real    (const R *H_N, const Q *Y_N1,                 Q *Y_N2, const int frame_id);
	void _tdemodulate_complex   (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_real      (              const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg_complex(const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);
	void _tdemodulate_wg_real   (const R *H_N, const Q *Y_N1,  const Q *Y_N2, Q *Y_N3, const int frame_id);

};
}
}

#include "Module/Modem/Generic/Modem_generic.hxx"

#endif // MODEM_GENERIC_HPP_
