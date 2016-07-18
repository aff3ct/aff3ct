#include <cassert>
#include <cmath>
#include <complex>
#include <limits>

#include "../mod_functions/Mod_Functions.hpp"
#include "Modulator_PSK.hpp"

/*
 * Constructor / Destructor
 */
template <typename B, typename R>
Modulator_PSK<B,R>
::Modulator_PSK(const unsigned int bits_per_symbol, const R sigma)
: bits_per_symbol(bits_per_symbol),
  nbr_symbols(1<<bits_per_symbol),
  sigma(sigma),
  sqrtEs(sqrt(2.0*(this->nbr_symbols-1)/3.0))
{
	mipp::vector<B> bits(this->bits_per_symbol);
	this->Constellation.resize(this->nbr_symbols);

	for (unsigned j = 0; j < this->nbr_symbols; j++)
	{
		for (unsigned l=0; l< this->bits_per_symbol; l++)
		{
			bits[l] = (j >> l) & 1;
		}

		this->Constellation[j] = this->bits_to_symbol(&bits[0]);
	}
}

template <typename B, typename R>
Modulator_PSK<B,R>
::~Modulator_PSK() {
}


/*
 * int get_buffer_size(const int N)
 * N = number of input bits
 * returns number of output symbols
 */
template <typename B, typename R>
int Modulator_PSK<B,R>
:: get_buffer_size(const int N)
{
	//assert(N % this->bits_per_symbol == 0);
	return std::ceil((float)N / (float)this->bits_per_symbol)*2;
}


/*
 * Mapping function
 */
template <typename B, typename R>
std::complex<R> Modulator_PSK<B,R>
::bits_to_symbol (const B* bits) const
 {
	auto bps = this->bits_per_symbol;
	R symbol;

	symbol = 1- ((R)bits[0] + (R)bits[0]);
	for (unsigned j=1; j<bps; j++)
	{
		symbol =  (1.0 - ((R)bits[j] + (R)bits[j])) * ((1<<j) - symbol);
	}
	return std::complex<R>(std::cos((symbol+1)*M_PI/this->nbr_symbols),std::sin((symbol+1)*M_PI/this->nbr_symbols));

 }

/*
 * Modulator
 */
template <typename B,typename R>
void Modulator_PSK<B,R>
:: modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) const
{
	auto size_out    = X_N2.size();
	auto size_in     = X_N1.size();
	auto bps         = this->bits_per_symbol;
	unsigned int idx = 0;
	auto size_rest   = size_in % bps;
	//std::complex<R> symbol;
	//std::cout << "Size In : " << size_in << std::endl;
	//std::cout << "Size Out : " << size_out << std::endl;
	for (unsigned i = 0; i < size_out/2-1; i++)
	{
		//symbol = bits_to_symbol(&X_N1[i*bps]);
		idx = 0;
		for (unsigned j = 0; j < bps; j++)
		{
			idx += (1 << j) * X_N1[i*bps + j];
		}
		 X_N2[2*i] = this->Constellation[idx].real();
		 X_N2[2*i+1] = this->Constellation[idx].imag();
	}

	idx = 0;
	for (unsigned j = 0; j < size_rest ;j++)
	{
		//std::cout<< "j= "<< j << std::endl;
		idx += (1 << j) * X_N1[((size_out/2-1)*bps + j)];
	}
	X_N2[size_out - 2] = this->Constellation[idx].real();
	X_N2[size_out - 1] = this->Constellation[idx].imag();
}

/*
 * Demodulator
 */
template <typename B,typename R>
void Modulator_PSK<B,R>
:: demodulate(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) const
{
	auto size = Y_N2.size();
	std::complex<R> complex_Yk;
	R L0;
	R L1;
	unsigned k, b;

	for (unsigned n = 0; n < size; n++)// Boucle sur les LLRs
	{
		L0 = -std::numeric_limits<R>::infinity();
		L1 = -std::numeric_limits<R>::infinity();
		b = n % this->bits_per_symbol; // position du bit
		k = n / this->bits_per_symbol; // Position du symbole

		complex_Yk =std::complex<R>(Y_N1[2*k],Y_N1[2*k+1]);

		R sigma2 = this->sigma*this->sigma;

		for (unsigned j = 0; j < this->nbr_symbols; j++)
		{
			if ( (j & (1 << b)) == 0 )
				L0 = max_star(L0,-std::norm(complex_Yk-this->Constellation[j])/(sigma2));
			else
				L1 = max_star(L1,-std::norm(complex_Yk-this->Constellation[j])/(sigma2));
		}
		Y_N2[n] = L0-L1;
	}
}



// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class Modulator_PSK<B_8,R_8>;
template class Modulator_PSK<B_16,R_16>;
template class Modulator_PSK<B_32,R_32>;
template class Modulator_PSK<B_64,R_64>;
#else
template class Modulator_PSK<B,R>;
#endif
// ==================================================================================== explicit template instantiation
