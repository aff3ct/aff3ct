#include "Modulator_PAM.hpp"

#include <cassert>
#include <cmath>
#include <complex>

#include "../mod_functions/Mod_Functions.hpp"

/*
 * Constructor / Destructor
 */
template <typename B, typename R>
Modulator_PAM<B,R>
::Modulator_PAM(const unsigned int bits_per_symbol, R sigma)
: bits_per_symbol(bits_per_symbol),
  nbr_symbols(1<<bits_per_symbol),
  sigma(sigma),
  sqrtEs(sqrt((this->nbr_symbols*this->nbr_symbols-1)/3.0))
{
}

template <typename B, typename R>
Modulator_PAM<B,R>
::~Modulator_PAM() {
}


/*
 * int get_buffer_size(const int N)
 * N = number of input bits
 * returns number of output symbols
 */
template <typename B, typename R>
int Modulator_PAM<B,R>
:: get_buffer_size(const int N)
{
	assert(N % this->bits_per_symbol == 0);
	return N / this->bits_per_symbol;
}


/*
 * Mapping function
 */
template <typename B, typename R>
R Modulator_PAM<B,R>
::bits_to_symbol (const B* bits) const
 {
	auto bps = this->bits_per_symbol;
	R symbol;

	symbol = 1- ((R)bits[0] + (R)bits[0]);
	for (unsigned j=1; j<bps; j++)
	{
		symbol =  (1.0 - ((R)bits[j] + (R)bits[j])) * ((1<<j) - symbol);
	}
	return symbol / this->sqrtEs;

 }

/*
 * Modulator
 */
template <typename B,typename R>
void Modulator_PAM<B,R>
:: modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const
{
	auto size = X_N2.size();
	auto bps = this->bits_per_symbol;
	for (unsigned i = 0; i < size; i++)
	{
		 X_N2[i] = bits_to_symbol(&X_N1[i*bps]);
	}
}

/*
 * Demodulator
 */
template <typename B,typename R>
void Modulator_PAM<B,R>
:: demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const
{
	auto size = Y_N2.size();
	mipp::vector<B> bits(this->bits_per_symbol);
	R current_symbol;
	mipp::vector<R> distances(this->nbr_symbols);
	R L0 = -INFINITY;
	R L1 = -INFINITY;
	unsigned k, b;

	for (unsigned n = 0; n < size; n++)// Boucle sur les LLRs
	{
		L0 = -INFINITY;
		L1 = -INFINITY;
		b = n % this->bits_per_symbol; // position du bit
		k = n / this->bits_per_symbol; // Position du symbole

		R sigma2 = this->sigma*this->sigma;

		for (unsigned j = 0; j < this->nbr_symbols; j++)
		{
			for (unsigned l=0; l< this->bits_per_symbol; l++)
			{
				bits[l] = (j >> l) & 1;
			}

			//current_symbol[0] = 0;
			current_symbol = this->bits_to_symbol(&bits[0]);
			//
			if (bits[b] == 0)
				L0 = max_star(L0,-(Y_N1[k]-current_symbol)*(Y_N1[k]-current_symbol)/(sigma2));
			else
				L1 = max_star(L1,-(Y_N1[k]-current_symbol)*(Y_N1[k]-current_symbol)/(sigma2));

		}
		Y_N2[n] = L0-L1;
	}

}



// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Modulator_PAM<B_8,R_8>;
template class Modulator_PAM<B_16,R_16>;
template class Modulator_PAM<B_32,R_32>;
template class Modulator_PAM<B_64,R_64>;
#else
template class Modulator_PAM<B,R>;
#endif
// ==================================================================================== explicit template instantiation
