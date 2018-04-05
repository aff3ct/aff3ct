#ifndef MODEM_OPTICAL_HPP_
#define MODEM_OPTICAL_HPP_

#include "Tools/Algo/Noise_generator/User_pdf_noise_generator/User_pdf_noise_generator.hpp"

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{

/* Optical modem is for optical communications.
 * There is a specific noise applied on sent bit at 0, and another one to 1.
 * The sigma variable from the inherited class represents the received optical power (ROP)
 */

template <typename B = int, typename R = float, typename Q = R>
class Modem_optical : public Modem<B,R,Q>
{
protected:
	tools::User_pdf_noise_generator<R> *noise_generator_p0;
	tools::User_pdf_noise_generator<R> *noise_generator_p1;

	R rop;

public:
	Modem_optical(const int N,
	              tools::User_pdf_noise_generator<R> *noise_generator_p0 = nullptr,
	              tools::User_pdf_noise_generator<R> *noise_generator_p1 = nullptr,
	              const R ROP = (R)-1.0,
	              const int n_frames = 1);
	virtual ~Modem_optical();

	virtual void set_sigma(const R ROP);

	static bool is_complex_mod()
	{
		return false;
	}

	static bool is_complex_fil()
	{
		return false;
	}

	static int size_mod(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_modulation(N, 1, 0, 1, is_complex_mod());
	}

	static int size_fil(const int N)
	{
		return Modem<B,R,Q>::get_buffer_size_after_filtering(N, 1, 0, 1, is_complex_fil());
	}


protected:
	void   _modulate    (const B *X_N1, R *X_N2, const int frame_id);
	void     _filter    (const R *Y_N1, R *Y_N2, const int frame_id);
	void _demodulate    (const Q *Y_N1, Q *Y_N2, const int frame_id);
};
}
}

#endif /* MODEM_OPTICAL_HPP_ */
