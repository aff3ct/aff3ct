#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>
#include <type_traits>
#include <complex>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/Generic/Modem_generic.hpp"

namespace aff3ct
{
namespace module
{
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_generic<B,R,Q,MAX>
::Modem_generic(const int N, const tools::Constellation<R>& _cstl, const bool disable_sig2)
: Modem<B,R,Q>(N,
               (int)(std::ceil((float)N / (float)_cstl.get_n_bits_per_symbol()) * (is_complex_mod(_cstl) ? 2 : 1))),
  cstl           (_cstl),
  bits_per_symbol(cstl.get_n_bits_per_symbol()),
  nbr_symbols    (cstl.get_n_symbols()),
  disable_sig2   (disable_sig2),
  inv_sigma2     ((R)1.)
{
	const std::string name = "Modem_generic<" + cstl.get_name() + ">";
	this->set_name(name);
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_generic<B,R,Q,MAX>* Modem_generic<B,R,Q,MAX>
::clone() const
{
	auto m = new Modem_generic(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
bool Modem_generic<B,R,Q,MAX>
::is_complex_mod(const tools::Constellation<R>& c)
{
	return c.is_complex();
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
bool Modem_generic<B,R,Q,MAX>
::is_complex_fil(const tools::Constellation<R>& c)
{
	return c.is_complex();
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
int Modem_generic<B,R,Q,MAX>
::size_mod(const int N, const tools::Constellation<R>& c)
{
	return Modem<B,R,Q>::get_buffer_size_after_modulation(N, c.get_n_bits_per_symbol(), 0, 1, is_complex_mod(c));
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
int Modem_generic<B,R,Q,MAX>
::size_fil(const int N, const tools::Constellation<R>& c)
{
	return Modem<B,R,Q>::get_buffer_size_after_filtering(N, c.get_n_bits_per_symbol(), 0, 1, is_complex_fil(c));
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_modulate(const B *X_N1, R *X_N2, const size_t frame_id)
{
	if (this->cstl.is_complex())
		_modulate_complex(X_N1, X_N2, frame_id);
	else
		_modulate_real(X_N1, X_N2, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tmodulate(const Q *X_N1, R *X_N2, const size_t frame_id)
{
	if (this->cstl.is_complex())
		_tmodulate_complex(X_N1, X_N2, frame_id);
	else
		_tmodulate_real(X_N1, X_N2, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->inv_sigma2 = (R)((R)1.0 / (2 * *CP * *CP));

	if (this->cstl.is_complex())
		_demodulate_complex(Y_N1, Y_N2, frame_id);
	else
		_demodulate_real(Y_N1, Y_N2, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->inv_sigma2 = (R)((R)1.0 / (2 * *CP * *CP));

	if (this->cstl.is_complex())
		_demodulate_wg_complex(H_N, Y_N1, Y_N2, frame_id);
	else
		_demodulate_wg_real(H_N, Y_N1, Y_N2, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->inv_sigma2 = (R)((R)1.0 / (2 * *CP * *CP));

	if (this->cstl.is_complex())
		_tdemodulate_complex(Y_N1, Y_N2, Y_N3, frame_id);
	else
		_tdemodulate_real(Y_N1, Y_N2, Y_N3, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (*CP != this->last_channel_param && !this->disable_sig2)
		this->inv_sigma2 = (R)((R)1.0 / (2 * *CP * *CP));

	if (this->cstl.is_complex())
		_tdemodulate_wg_complex(H_N, Y_N1, Y_N2, Y_N3, frame_id);
	else
		_tdemodulate_wg_real(H_N, Y_N1, Y_N2, Y_N3, frame_id);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_modulate_complex(const B *X_N1, R *X_N2, const size_t frame_id)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;

	auto loop_size = size_in / this->bits_per_symbol;
	for (auto i = 0; i < loop_size; i++)
	{
		unsigned idx = 0;
		for (auto j = 0; j < this->bits_per_symbol; j++)
			idx += unsigned(unsigned(1 << j) * X_N1[i * this->bits_per_symbol +j]);
		auto symbol = cstl[idx];

		X_N2[2*i   ] = symbol.real();
		X_N2[2*i +1] = symbol.imag();
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (loop_size * this->bits_per_symbol < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (loop_size * this->bits_per_symbol); j++)
			idx += unsigned(unsigned(1 << j) * X_N1[loop_size * this->bits_per_symbol +j]);
		auto symbol = cstl[idx];

		X_N2[size_out -2] = symbol.real();
		X_N2[size_out -1] = symbol.imag();
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate_complex(const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk - std::complex<Q>((Q)cstl[j].real(),
				                                                     (Q)cstl[j].imag())) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk - std::complex<Q>((Q)cstl[j].real(),
				                                                     (Q)cstl[j].imag())) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate_wg_complex(const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(   Y_N1[2*k],    Y_N1[2*k+1]);
		auto complex_Hk = std::complex<Q>((Q)H_N [2*k], (Q)H_N [2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)cstl[j].real(),
				                                                     (Q)cstl[j].imag())) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)cstl[j].real(),
				                                                     (Q)cstl[j].imag())) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate_complex(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		auto complex_Yk = std::complex<Q>(Y_N1[2*k], Y_N1[2*k+1]);

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = (Q)(std::norm(complex_Yk - std::complex<Q>((Q)cstl[j].real(),
			                                                        (Q)cstl[j].imag())) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (((j>>l) & 1) == 1)
				{
					if (k * this->bits_per_symbol +l < size)
						tempL += Y_N2[k * this->bits_per_symbol +l];
					else
						tempL += std::numeric_limits<Q>::infinity();
				}

			}
			tempL = std::isnan((R)tempL) ? (Q)0.0 : tempL;

			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate_wg_complex(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

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
			                           complex_Hk * std::complex<Q>((Q)cstl[j].real(),
			                                                        (Q)cstl[j].imag())) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (((j>>l) & 1) == 1)
				{
					if (k * this->bits_per_symbol +l < size)
						tempL += Y_N2[k * this->bits_per_symbol +l];
					else
						tempL += std::numeric_limits<Q>::infinity();
				}
			}
			tempL = std::isnan((R)tempL) ? (Q)0.0 : tempL;

			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B, R, Q, MAX>
::_tmodulate_complex(const Q *X_N1, R *X_N2, const size_t frame_id)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;

	auto loop_size = size_in / this->bits_per_symbol;

	for (auto i = 0; i < loop_size; i++)
	{
		X_N2[2*i+0] = (R)0.0;
		X_N2[2*i+1] = (R)0.0;

		for (auto m = 0; m < this->nbr_symbols; m++)
		{
			const auto& soft_symbol = cstl[m];
			auto p = (R)1.0;
			for (auto j = 0; j < this->bits_per_symbol; j++)
			{
				auto p0 = (R)1.0/((R)1.0 + std::exp(-(R)(X_N1[i*this->bits_per_symbol + j])));
				p *= ((m >> j) & 1) == 0 ? p0 : (R)1.0 - p0;
			}
			X_N2[2*i]   += p * soft_symbol.real();
			X_N2[2*i+1] += p * soft_symbol.imag();
		}
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (loop_size * this->bits_per_symbol < size_in)
	{
		auto r = size_in - (loop_size * this->bits_per_symbol);
		X_N2[size_out - 2] = (R)0.0;
		X_N2[size_out - 1] = (R)0.0;

		for (auto m = 0; m < (1<<r); m++)
		{
			const auto& soft_symbol = cstl[m];
			auto p = (R)1.0;
			for (auto j = 0; j < r; j++)
			{
				auto p0 = (R)1.0/((R)1.0 + std::exp(-(R)X_N1[loop_size*this->bits_per_symbol + j]));
				p *= ((m >> j) & 1) == 0 ? p0 : (R)1.0 - p0;
			}
			X_N2[size_out - 2] += p*soft_symbol.real();
			X_N2[size_out - 1] += p*soft_symbol.imag();
		}
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_modulate_real(const B *X_N1, R *X_N2, const size_t frame_id)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;
	auto bps      = this->bits_per_symbol;

	auto main_loop_size = size_in / bps;
	for (auto i = 0; i < main_loop_size; i++)
	{
		// determine the symbol with a lookup table
		unsigned idx = 0;
		for (auto j = 0; j < bps; j++)
			idx += unsigned(unsigned(1 << j) * X_N1[i * bps +j]);
		auto symbol = cstl.get_real(idx);

		X_N2[i] = symbol;
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (main_loop_size * bps < size_in)
	{
		unsigned idx = 0;
		for (auto j = 0; j < size_in - (main_loop_size * bps); j++)
			idx += unsigned(unsigned(1 << j) * X_N1[main_loop_size * bps +j]);
		auto symbol = cstl.get_real(idx);

		X_N2[size_out -1] = symbol;
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate_real(const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -(Y_N1[k] - (Q)cstl.get_real(j)) *
				              (Y_N1[k] - (Q)cstl.get_real(j)) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -(Y_N1[k] - (Q)cstl.get_real(j)) *
				              (Y_N1[k] - (Q)cstl.get_real(j)) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_demodulate_wg_real(const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -(Y_N1[k] - (Q)H_N[k] * (Q)cstl.get_real(j)) *
				              (Y_N1[k] - (Q)H_N[k] * (Q)cstl.get_real(j)) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -(Y_N1[k] - (Q)H_N[k] * (Q)cstl.get_real(j)) *
				              (Y_N1[k] - (Q)H_N[k] * (Q)cstl.get_real(j)) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate_real(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // loop upon the LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL  = (Q)((Y_N1[k] - cstl.get_real(j)) *
			                  (Y_N1[k] - cstl.get_real(j)) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (((j>>l) & 1) == 1)
				{
					if (k * this->bits_per_symbol +l < size)
						tempL += Y_N2[k * this->bits_per_symbol +l];
					else
						tempL += std::numeric_limits<Q>::infinity();
				}

			}
			tempL = std::isnan((R)tempL) ? (Q)0.0 : tempL;

			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B,R,Q,MAX>
::_tdemodulate_wg_real(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = this->N;

	for (auto n = 0; n < size; n++) // boucle sur les LLRs
	{
		auto L0 = -std::numeric_limits<Q>::infinity();
		auto L1 = -std::numeric_limits<Q>::infinity();
		auto b  = n % this->bits_per_symbol; // bit position in the symbol
		auto k  = n / this->bits_per_symbol; // symbol position

		for (auto j = 0; j < this->nbr_symbols; j++)
		{
			auto tempL = (Q)((Y_N1[k] - (Q)H_N[k] * cstl.get_real(j)) *
			                 (Y_N1[k] - (Q)H_N[k] * cstl.get_real(j)) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (((j>>l) & 1) == 1)
				{
					if (k * this->bits_per_symbol +l < size)
						tempL += Y_N2[k * this->bits_per_symbol +l];
					else
						tempL += std::numeric_limits<Q>::infinity();
				}
			}
			tempL = std::isnan((R)tempL) ? (Q)0.0 : tempL;

			if (((j>>b) & 1) == 0)
				L0 = MAX(L0, -tempL);
			else
				L1 = MAX(L1, -tempL);
		}

		Y_N3[n] = (L0 - L1);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_generic<B, R, Q, MAX>
::_tmodulate_real(const Q *X_N1, R *X_N2, const size_t frame_id)
{
	auto size_in  = this->N;
	auto size_out = this->N_mod;

	auto loop_size = size_in / this->bits_per_symbol;

	for (auto i = 0; i < loop_size; i++)
	{
		X_N2[i] = (R)0.0;

		for (auto m = 0; m < this->nbr_symbols; m++)
		{
			R soft_symbol = cstl.get_real(m);
			R p = 1.0;
			for (auto j = 0; j < this->bits_per_symbol; j++)
			{
				R p0 = (R)1.0/((R)1.0 + std::exp(-(R)(X_N1[i*this->bits_per_symbol + j])));
				p *= ((m >> j) & 1) == 0 ? p0 : (R)1.0 - p0;
			}
			X_N2[i]   += p * soft_symbol;
		}
	}

	// last elements if "size_in" is not a multiple of the number of bits per symbol
	if (loop_size * this->bits_per_symbol < size_in)
	{
		auto r = size_in - (loop_size * this->bits_per_symbol);
		X_N2[size_out - 1] = (R)0.0;

		for (auto m = 0; m < (1<<r); m++)
		{
			R soft_symbol = cstl.get_real(m);
			auto p = (R)1.0;
			for (auto j = 0; j < r; j++)
			{
				auto p0 = (R)1.0/((R)1.0 + std::exp(-(R)X_N1[loop_size*this->bits_per_symbol + j]));
				p *= ((m >> j) & 1) == 0 ? p0 : (R)1.0 - p0;
			}
			X_N2[size_out - 1] += p*soft_symbol;
		}
	}
}
}
}
