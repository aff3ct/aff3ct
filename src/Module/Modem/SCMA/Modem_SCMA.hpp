#ifndef MODEM_SCMA_HPP_
#define MODEM_SCMA_HPP_

#include <complex>
#include <vector>

#include "Tools/Code/SCMA/modem_SCMA_functions.hpp"

#include "../Modem.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, tools::proto_psi<Q> PSI = tools::psi_0>
class Modem_SCMA : public Modem<B,R,Q>
{
private:
	const static std::complex<float> CB[6][4][4];
	const int                        re_user[4][3]       = {{1,2,4},{0,2,5},{1,3,5},{0,3,4}};
	      Q                          arr_phi[4][4][4][4] = {}; // probability functions
	const bool                       disable_sig2;
	      R                          n0; // 1 / n0 = 179.856115108
	const int                        n_ite;

public:
	Modem_SCMA(const int N, const R sigma = (R)1, const int bps = 3, const bool disable_sig2 = false,
	           const int n_ite = 1, const int n_frames = 6);
	virtual ~Modem_SCMA();

	virtual void set_sigma(const R sigma);

	virtual void modulate     (              const B* X_N1, R *X_N2, const int frame_id = -1); using Modem<B,R,Q>::modulate;
	virtual void demodulate   (              const Q *Y_N1, Q *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::demodulate;
	virtual void demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::demodulate_wg;
	virtual void filter       (              const R *Y_N1, R *Y_N2, const int frame_id = -1); using Modem<B,R,Q>::filter;

	static int size_mod(const int N, const int bps)
	{
		return ((int)std::pow(2,bps) * ((N +1) / 2));
	}

	static int size_fil(const int N, const int bps)
	{
		return size_mod(N, bps);
	}

private:
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch);
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N);
	void demodulate_batch(const Q* Y_N1, Q* Y_N2, int batch);
};
}
}

#include "Modem_SCMA.hxx"

#endif /* MODEM_SCMA_HPP_ */
