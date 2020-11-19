/*!
 * \file
 * \brief Class module::Modem_BPSK.
 */
#ifndef MODEM_BPSK_HPP_
#define MODEM_BPSK_HPP_

#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_BPSK : public Modem<B,R,Q>
{
private:
	const bool disable_sig2;
	R two_on_square_sigma;

public:
	explicit Modem_BPSK(const int N, const bool disable_sig2 = false);
	virtual ~Modem_BPSK() = default;

	virtual Modem_BPSK<B,R,Q>* clone() const;

	static bool is_complex_mod();
	static bool is_complex_fil();
	static int size_mod(const int N);
	static int size_fil(const int N);

protected:
	void   _modulate    (                               const B *X_N1,                R *X_N2, const size_t frame_id);
	void     _filter    (const float *CP,               const R *Y_N1,                R *Y_N2, const size_t frame_id);
	void _demodulate    (const float *CP,               const Q *Y_N1,                Q *Y_N2, const size_t frame_id);
	void _demodulate_wg (const float *CP, const R *H_N, const Q *Y_N1,                Q *Y_N2, const size_t frame_id);
	void _tdemodulate   (const float *CP,               const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);
	void _tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Modem/BPSK/Modem_BPSK.hxx"
#endif

#endif /* MODEM_BPSK_HPP_ */
