#include <cassert>
#include <cmath>
#include <complex>
#include <limits>
#include <fstream>
#include <iterator>

#include "Modulator_USR.hpp"

/*
 * Constructor / Destructor
 */
template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_USR<B,R,Q,MAX>
::Modulator_USR(const int N, const int bits_per_symbol, const std::string const_path, const R sigma,
                const bool disable_sig2, const int n_frames, const std::string name)
: Modulator<B,R,Q>(N, std::ceil((float)N / (float)bits_per_symbol) * 2, n_frames, name),
  bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
  sigma          (sigma),
  sqrt_es        (0.0),
  disable_sig2   (disable_sig2),
  constellation  ()
{
	std::fstream const_file(const_path, std::ios_base::in);

	std::string temp;
	while (std::getline(const_file, temp))
	{
		if (temp[0] == '#') continue;

		std::istringstream buffer(temp);
		std::vector<R> line((std::istream_iterator<R>(buffer)), std::istream_iterator<R>());
		assert (line.size() < 3);

		if (line.size() == 2)
			constellation.push_back(std::complex<R>(line[0],line[1]));
		else
			constellation.push_back(std::complex<R>(line[0],0.0));
		sqrt_es += std::norm(constellation.back());
	}
	sqrt_es = sqrt(sqrt_es/nbr_symbols);

	assert (constellation.size() == nbr_symbols);

	for (auto i = 0; i < nbr_symbols; i++)
		constellation[i] /= (std::complex<R>)sqrt_es;

	const_file.close();
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_USR<B,R,Q,MAX>
::~Modulator_USR()
{
}

/*
 * int get_buffer_size_after_modulation(const int N)
 * N = number of input bits
 * returns number of output symbols
 */
template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_USR<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
	assert(this->bits_per_symbol % 2 == 0);
	return std::ceil((float)N / (float)this->bits_per_symbol) * 2;
}

/*
 * Modulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_USR<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	auto size_in  = (int)X_N1.size();
	auto size_out = (int)X_N2.size();
	auto bps      = this->bits_per_symbol;

	auto main_loop_size = size_in / bps;
	for (auto i = 0; i < main_loop_size; i++)
	{
		unsigned idx = 0;
		for (auto j = 0; j < bps; j++)
			idx += (1 << (bps-j-1)) * X_N1[i * bps +j];
		auto symbol = this->constellation[idx];

		X_N2[2*i   ] = symbol.real();
		X_N2[2*i +1] = symbol.imag();
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (main_loop_size * bps < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (main_loop_size * bps); j++)
			idx += (1 << (bps-j-1)) * X_N1[main_loop_size * bps +j];
		auto symbol = this->constellation[idx];

		X_N2[size_out -2] = symbol.real();
		X_N2[size_out -1] = symbol.imag();
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, proto_max<Q> MAX>
void Modulator_USR<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));
	
	auto size       = (int)Y_N2.size();
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)1.0 / (this->sigma * this->sigma);

	for (auto n = 0; n < size; n++) // boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();

		auto b  = n % this->bits_per_symbol; // position du bit
		auto k  = n / this->bits_per_symbol; // position du symbole

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if ((j &(1 << (this->bits_per_symbol -b -1))) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk - std::complex<Q>(this->constellation[j])) * inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk - std::complex<Q>(this->constellation[j])) * inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_USR<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert(typeid(R) == typeid(Q));
	assert(typeid(Q) == typeid(float) || typeid(Q) == typeid(double));

	auto size       = (int)Y_N3.size();
	auto inv_sigma2 = disable_sig2 ? (Q)1.0 : (Q)1.0 / (this->sigma * this->sigma);

	for (auto n = 0; n < size; n++) // boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // position du bit
		auto k  = n / this->bits_per_symbol; // position du symbole

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = std::norm(complex_Yk - std::complex<Q>(this->constellation[j])) * inv_sigma2;
			for (auto l = 0; l < b; l++)
				tempL += (j & (1 << (this->bits_per_symbol -l -1))) * Y_N2[k * this->bits_per_symbol +l];

			for (auto l = b + 1; l < this->bits_per_symbol; l++)
				tempL += (j & (1 << (this->bits_per_symbol-l-1))) * Y_N2[k * this->bits_per_symbol +l];

			if ((j & (1 << (this->bits_per_symbol-b-1))) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}


