#ifndef MODULATOR_SCMA_HPP_
#define MODULATOR_SCMA_HPP_

#include <complex>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "modulator_SCMA_functions.hpp"

#include "../Modulator.hpp"

namespace aff3ct
{
namespace module
{
/*
 * Constructor / Destructor
 */
template <typename B = int, typename R = float, typename Q = R, tools::proto_psi<Q> PSI = tools::psi_0>
class Modulator_SCMA : public Modulator<B,R,Q>
{
private:
	const static std::complex<float> CB[6][4][4];
	const        int                 re_user[4][3]       = {{1,2,4},{0,2,5},{1,3,5},{0,3,4}};
	             Q                   arr_phi[4][4][4][4] = {}; // probability functions

	const bool                       disable_sig2;
	const R                          two_on_square_sigma;
	const R                          n0; // 1 / n0 = 179.856115108
	const int                        n_ite;

public:
	Modulator_SCMA(const int N, const R sigma, const bool disable_sig2 = false, const int n_ite = 1, 
	               const std::string name = "Modulator_SCMA");
	virtual ~Modulator_SCMA();

	virtual void modulate             (const B* X_N1, R* X_N2);
	virtual void demodulate           (const Q *Y_N1, Q *Y_N2);
	virtual void demodulate_with_gains(const Q *Y_N1, const R *H_N, Q *Y_N2);
	virtual void _filter              (const R *Y_N1, R *Y_N2, const int frame_id);

	virtual void set_n_frames(const int n_frames);


	static int size_mod(const int N)
	{
		// TODO move elsewhere
		// if(bps != 3)
		// 	throw std::invalid_argument("aff3ct::module::Modulator_SCMA: \"bit_per_symbol\" value is invalid.");

		return (8 * ((N + 1) / 2));
	}

	static int size_fil(const int N)
	{
		return size_mod(N);
	}

private:
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch);
	Q phi(const Q* Y_N1, int i, int j, int k, int re, int batch, const R* H_N);
	void demodulate_batch(const Q* Y_N1, Q* Y_N2, int batch);
};
}
}

#include "Modulator_SCMA.hxx"

#endif /* MODULATOR_SCMA_HPP_ */
