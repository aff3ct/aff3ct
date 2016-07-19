#ifndef MODULATOR_PSK_HPP_
#define MODULATOR_PSK_HPP_

#include <complex>
#include <vector>
#include "../../Tools/MIPP/mipp.h"
#include "../Modulator.hpp"

template <typename B, typename R>
class Modulator_PSK : public Modulator<B,R>
{
private:
	const int bits_per_symbol;
	const int nbr_symbols;
	const R sigma;
	mipp::vector<std::complex<R>> constellation;

public:
	Modulator_PSK(const int bits_per_symbol = 2, const R sigma = 1.0);
    virtual ~Modulator_PSK();

	virtual void   modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const;
	
	int get_buffer_size(const int N);

private:
	inline std::complex<R> bits_to_symbol(const B* bits) const;
};

#endif // MODULATOR_PSK_HPP_
