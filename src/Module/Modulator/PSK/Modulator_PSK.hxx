#include <cassert>
#include <cmath>
#include <complex>
#include <limits>

#include "Modulator_PSK.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/*
 * Constructor / Destructor
 */
template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_PSK<B,R,Q,MAX>
::Modulator_PSK(const int N, mipp::vector<R> &H, const int bits_per_symbol, const R sigma, const bool disable_sig2,
                const int n_frames, const std::string name)
: Modulator<B,R,Q>(N, 
                   (int)std::ceil((float)N / (float)bits_per_symbol) * 2,
                   H,
                   n_frames,
                   name),
  bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
  sigma          (sigma),
  disable_sig2   (disable_sig2),
  constellation  (nbr_symbols)
{
	mipp::vector<B> bits(this->bits_per_symbol);

	for (auto j = 0; j < this->nbr_symbols; j++)
	{
		for (auto l = 0; l< this->bits_per_symbol; l++)
			bits[l] = (j >> l) & 1;

		this->constellation[j] = this->bits_to_symbol(&bits[0]);
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_PSK<B,R,Q,MAX>
::~Modulator_PSK()
{
}

/*
 * int get_buffer_size_after_modulation(const int N)
 * N = number of input bits
 * returns number of output symbols
 */
template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_PSK<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
	return (int)std::ceil((float)N / (float)this->bits_per_symbol) * 2;
}

/*
 * Mapping function
 */
template <typename B, typename R, typename Q, proto_max<Q> MAX>
std::complex<R> Modulator_PSK<B,R,Q,MAX>
::bits_to_symbol(const B* bits) const
{
	auto bps = this->bits_per_symbol;

	auto symbol = (R)1.0 - ((R)bits[0] + (R)bits[0]);
	for (auto j = 1; j < bps; j++)
		symbol = ((R)1.0 - ((R)bits[j] + (R)bits[j])) * ((1 << j) - symbol);

	return std::complex<R>((R)std::cos((symbol +1) * M_PI / this->nbr_symbols), 
	                       (R)std::sin((symbol +1) * M_PI / this->nbr_symbols));
}

/*
 * Modulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	auto size_in  = (int)X_N1.size();
	auto size_out = (int)X_N2.size();
	auto bps      = this->bits_per_symbol;

	auto main_loop_size = size_in / bps;
	for (auto i = 0; i < main_loop_size; i++)
	{
		// compute the symbol "on the fly"
		// auto symbol = bits_to_symbol(&X_N1[i*bps]);

		// determine the symbol with a lookup table
		unsigned idx = 0;
		for (auto j = 0; j < bps; j++)
			idx += unsigned(unsigned(1 << j) * X_N1[i * bps +j]);
		auto symbol = this->constellation[idx];

		X_N2[2*i +0] = symbol.real();
		X_N2[2*i +1] = symbol.imag();
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (main_loop_size * bps < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (main_loop_size * bps); j++)
			idx += unsigned(unsigned(1 << j) * X_N1[main_loop_size * bps +j]);
		auto symbol = this->constellation[idx];

		X_N2[size_out -2] = symbol.real();
		X_N2[size_out -1] = symbol.imag();
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));
	
	auto size       = (int)Y_N2.size();
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)(1.0 / (this->sigma * this->sigma));

	for (auto n = 0; n < size; n++)// boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // position du bit
		auto k  = n / this->bits_per_symbol; // position du symbole

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if ((j & (1 << b)) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	auto size       = (int)Y_N3.size();
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)1.0 / (this->sigma * this->sigma);

	for (auto n = 0; n < size; n++)// boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // position du bit
		auto k  = n / this->bits_per_symbol; // position du symbole

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = (Q)(std::norm(complex_Yk - std::complex<Q>(this->constellation[j])) * inv_sigma2);
			for (auto l = 0; l < b; l++)
				tempL += (j & (1 << l)) * Y_N2[k * this->bits_per_symbol +l];

			for (auto l = b +1; l < this->bits_per_symbol; l++)
				tempL += (j & (1 << l)) * Y_N2[k * this->bits_per_symbol +l];

			if ((j & (1 << b)) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}
