#include <stdexcept>
#include <cmath>
#include <complex>
#include <limits>

#include "Modulator_PSK.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace aff3ct
{
namespace module
{
/*
 * Constructor / Destructor
 */
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modulator_PSK<B,R,Q,MAX>
::Modulator_PSK(const int N, const R sigma, const int bits_per_symbol, const bool disable_sig2, const int n_frames,
                const std::string name)
: Modulator<B,R,Q>(N, 
                   (int)std::ceil((float)N / (float)bits_per_symbol) * 2,
                   sigma,
                   n_frames,
                   name),
  bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
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

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modulator_PSK<B,R,Q,MAX>
::~Modulator_PSK()
{
}

/*
 * Mapping function
 */
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
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
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_modulate(const B *X_N1, R *X_N2)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;
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
 * Filter
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_filter(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_demodulate(const Q *Y_N1, Q *Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"Q\" has to be float or double.");
	
	auto size       = this->N;
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)(1.0 / (this->sigma * this->sigma));

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

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
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_demodulate_with_gains(const Q *Y_N1, const R *H_N, Q *Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"Q\" has to be float or double.");

	auto size       = this->N;
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)(1.0 / (this->sigma * this->sigma));

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(   Y_N1[2*k],    Y_N1[2*k+1]);
		auto complex_Hk = std::complex<Q>((Q)H_N [2*k], (Q)H_N [2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if ((j & (1 << b)) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_demodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"Q\" has to be float or double.");

	auto size       = this->N;
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)1.0 / (this->sigma * this->sigma);

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = (Q)(std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
			                                                        (Q)this->constellation[j].imag())) * inv_sigma2);

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

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_PSK<B,R,Q,MAX>
::_demodulate_with_gains(const Q *Y_N1, const R *H_N, const Q *Y_N2, Q *Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_PSK: type \"Q\" has to be float or double.");

	auto size       = this->N;
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)1.0 / (this->sigma * this->sigma);

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(   Y_N1[2*k],    Y_N1[2*k+1]);
		auto complex_Hk = std::complex<Q>((Q)H_N [2*k], (Q)H_N [2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = (Q)(std::norm(complex_Yk -
			                           complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
			                                                        (Q)this->constellation[j].imag())) * inv_sigma2);

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
}
}
