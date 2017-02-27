#ifndef MODULATOR_USER_HPP_
#define MODULATOR_USER_HPP_

#include <complex>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "../Modulator.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float, typename Q = R, proto_max<Q> MAX = tools::max_star>
class Modulator_user : public Modulator<B,R,Q>
{
private:
	const int bits_per_symbol;
	const int nbr_symbols;
	const R sigma;
	R sqrt_es;
	const bool disable_sig2;
	mipp::vector<std::complex<R>> constellation;

public:
	Modulator_user(const int N, const R sigma, const int bits_per_symbol = 2, const std::string const_path = "",
	               const bool disable_sig2 = false, const int n_frames = 1, const std::string name = "Modulator_user");
	virtual ~Modulator_user();

	void   modulate           (const mipp::vector<B>& X_N1,                                                          mipp::vector<R>& X_N2);
	void demodulate           (const mipp::vector<Q>& Y_N1,                                                          mipp::vector<Q>& Y_N2);
	void demodulate_with_gains(const mipp::vector<Q>& Y_N1, const mipp::vector<R>& H_N,                              mipp::vector<Q>& Y_N2);
	void demodulate           (const mipp::vector<Q>& Y_N1,                             const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);
	void demodulate_with_gains(const mipp::vector<Q>& Y_N1, const mipp::vector<R>& H_N, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);

	int get_buffer_size_after_modulation(const int N);
};
}
}

#include "Modulator_user.hxx"

#endif // MODULATOR_USER_HPP_
