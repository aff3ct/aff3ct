#ifndef MODEM_OOK_HPP_
#define MODEM_OOK_HPP_

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R>
class Modem_OOK : public Modem<B,R,Q>
{
public:
	Modem_OOK(const int N, const tools::Noise<R>& noise, const int n_frames = 1);
	virtual ~Modem_OOK() = default;

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
	void _modulate(const B *X_N1, R *X_N2, const int frame_id);
	void   _filter(const R *Y_N1, R *Y_N2, const int frame_id);
};
}
}

#endif /* MODEM_OOK_HPP_ */
