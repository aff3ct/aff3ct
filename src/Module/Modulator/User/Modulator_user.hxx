#include <stdexcept>
#include <cmath>
#include <complex>
#include <limits>
#include <fstream>
#include <iterator>

#include "Modulator_user.hpp"

namespace aff3ct
{
namespace module
{
/*
 * Constructor / Destructor
 */
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modulator_user<B,R,Q,MAX>
::Modulator_user(const int N, const R sigma, const int bits_per_symbol, const std::string const_path,
                 const bool disable_sig2, const int n_frames, const std::string name)
: Modulator<B,R,Q>(N, (int)(std::ceil((float)N / (float)bits_per_symbol) * 2), n_frames, name),
  bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
  sigma          (sigma),
  sqrt_es        (0.0),
  disable_sig2   (disable_sig2),
  constellation  ()
{
	if (const_path.empty())
		throw std::invalid_argument("aff3ct::module::Modulator_user: path to the constellation file should not "
		                            "be empty.");

	if (this->bits_per_symbol % 2)
		throw std::invalid_argument("aff3ct::module::Modulator_user: \"bits_per_symbol\" has to be a multiple of 2.");

	std::fstream const_file(const_path, std::ios_base::in);

	std::string temp;
	while (std::getline(const_file, temp))
	{
		if (temp[0] == '#') continue;

		std::istringstream buffer(temp);
		std::vector<R> line((std::istream_iterator<R>(buffer)), std::istream_iterator<R>());

		if (line.size() >= 3)
			throw std::runtime_error("aff3ct::module::Modulator_user: \"line.size()\" has to be smaller than 3.");

		if (line.size() == 2)
			constellation.push_back(std::complex<R>(line[0],line[1]));
		else
			constellation.push_back(std::complex<R>(line[0],0.0));
		sqrt_es += std::norm(constellation.back());
	}
	sqrt_es = std::sqrt(sqrt_es/nbr_symbols);

	if ((int)constellation.size() != nbr_symbols)
		throw std::runtime_error("aff3ct::module::Modulator_user: \"constellation.size()\" has to be equal to "
		                         "\"nbr_symbols\".");

	for (auto i = 0; i < nbr_symbols; i++)
		constellation[i] /= (std::complex<R>)sqrt_es;

	const_file.close();
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modulator_user<B,R,Q,MAX>
::~Modulator_user()
{
}

/*
 * Modulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_user<B,R,Q,MAX>
::_modulate(const B *X_N1, R *X_N2)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;

	auto loop_size = size_in / this->bits_per_symbol;
	for (auto i = 0; i < loop_size; i++)
	{
		unsigned idx = 0;
		for (auto j = 0; j < this->bits_per_symbol; j++)
			idx += unsigned(unsigned(1 << (this->bits_per_symbol -j -1)) * X_N1[i * this->bits_per_symbol +j]);
		auto symbol = this->constellation[idx];

		X_N2[2*i   ] = symbol.real();
		X_N2[2*i +1] = symbol.imag();
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (loop_size * this->bits_per_symbol < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (loop_size * this->bits_per_symbol); j++)
			idx += unsigned(unsigned(1 << (this->bits_per_symbol -j -1)) * X_N1[loop_size * this->bits_per_symbol +j]);
		auto symbol = this->constellation[idx];

		X_N2[size_out -2] = symbol.real();
		X_N2[size_out -1] = symbol.imag();
	}
}

/*
 * Filter
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_user<B,R,Q,MAX>
::_filter(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_user<B,R,Q,MAX>
::_demodulate(const Q *Y_N1, Q *Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"Q\" has to be float or double.");

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
			if ((j &(1 << (this->bits_per_symbol -b -1))) == 0)
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
void Modulator_user<B,R,Q,MAX>
::_demodulate_with_gains(const Q *Y_N1, const R *H_N, Q *Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"Q\" has to be float or double.");

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
			if ((j &(1 << (this->bits_per_symbol -b -1))) == 0)
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

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_user<B,R,Q,MAX>
::_demodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"Q\" has to be float or double.");

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
				tempL += (j & (1 << (this->bits_per_symbol -l -1))) * Y_N2[k * this->bits_per_symbol +l];

			for (auto l = b + 1; l < this->bits_per_symbol; l++)
				tempL += (j & (1 << (this->bits_per_symbol -l -1))) * Y_N2[k * this->bits_per_symbol +l];

			if ((j & (1 << (this->bits_per_symbol-b-1))) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1) + Y_N2[n];
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modulator_user<B,R,Q,MAX>
::_demodulate_with_gains(const Q *Y_N1, const R *H_N, const Q *Y_N2, Q *Y_N3)
{
	if (typeid(R) != typeid(Q))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"R\" and \"Q\" have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw std::invalid_argument("aff3ct::module::Modulator_user: type \"Q\" has to be float or double.");

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
				tempL += (j & (1 << (this->bits_per_symbol -l -1))) * Y_N2[k * this->bits_per_symbol +l];

			for (auto l = b + 1; l < this->bits_per_symbol; l++)
				tempL += (j & (1 << (this->bits_per_symbol -l -1))) * Y_N2[k * this->bits_per_symbol +l];

			if ((j & (1 << (this->bits_per_symbol-b-1))) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1) + Y_N2[n];
	}
}
}
}
