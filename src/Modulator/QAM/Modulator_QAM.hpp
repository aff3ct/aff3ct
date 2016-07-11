#ifndef MODULATOR_QAM_HPP_
#define MODULATOR_QAM_HPP_

#include <vector>
#include <cmath>
#include <complex>
#include "../../Tools/MIPP/mipp.h"
#include "../Modulator.hpp"

template <typename B, typename R>
class Modulator_QAM : public Modulator<B,R>
{
public:

	Modulator_QAM(const unsigned int bits_per_symbol = 2,const R sigma = 1.0);
    virtual ~Modulator_QAM();

	virtual void modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const;
	virtual void demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const;
	int get_buffer_size(const int N);

	int bits_per_symbol;
	int nbr_symbols;
	R sigma;
	R sqrtEs;

private:
	std::complex<R> bits_to_symbol (const B* bits) const;
};

#endif // MODULATOR_QAM_HPP_
