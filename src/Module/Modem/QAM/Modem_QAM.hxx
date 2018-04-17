#include <cmath>
#include <complex>
#include <limits>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Modem_QAM.hpp"

namespace aff3ct
{
namespace module
{
/*
 * Constructor / Destructor
 */
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_QAM<B,R,Q,MAX>
::Modem_QAM(const int N, const tools::Noise<R>& noise, const int bits_per_symbol,
            const bool disable_sig2, const int n_frames)
: Modem<B,R,Q>(N,
               (int)std::ceil((float)N / (float)bits_per_symbol) * 2,
               noise,
               n_frames),
  bits_per_symbol(bits_per_symbol),
  nbr_symbols    (1 << bits_per_symbol),
  sqrt_es        ((R)std::sqrt(2.0 * (this->nbr_symbols -1) / 3.0)),
  disable_sig2   (disable_sig2),
  constellation  (nbr_symbols)
{
	const std::string name = "Modem_QAM";
	this->set_name(name);

	if (this->bits_per_symbol % 2)
	{
		std::stringstream message;
		message << "'bits_per_symbol' has to be a multiple of 2 ('bits_per_symbol' = " << bits_per_symbol << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<B> bits(this->bits_per_symbol);

	for (auto j = 0; j < this->nbr_symbols; j++)
	{
		for (auto l = 0; l < this->bits_per_symbol; l++)
			bits[l] = (j >> l) & 1;

		this->constellation[j] = this->bits_to_symbol(&bits[0]);
	}
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
Modem_QAM<B,R,Q,MAX>
::~Modem_QAM()
{
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
std::complex<R> Modem_QAM<B,R,Q,MAX>
::bits_to_symbol(const B* bits) const
{
	auto bps = this->bits_per_symbol;

	auto symbol_I = (R)1.0 - (bits[      0] + bits[      0]);
	auto symbol_Q = (R)1.0 - (bits[bps / 2] + bits[bps / 2]);

	for (auto j = 1; j < bps / 2; j++)
	{
		symbol_I = ((R)1.0 - (bits[         j] + bits[         j])) * ((1 << j) - symbol_I);
		symbol_Q = ((R)1.0 - (bits[bps / 2 +j] + bits[bps / 2 +j])) * ((1 << j) - symbol_Q);
	}

	return std::complex<R>(symbol_I, symbol_Q) / (std::complex<R>)this->sqrt_es;
}

template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::set_noise(const tools::Noise<R>& noise)
{
	Modem<B,R,Q>::set_noise(noise);

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);

	this->inv_sigma2 = this->disable_sig2 ?
	                    (R)1.0 :
	                    (R)((R)1.0 / (2 * this->n->get_noise() * this->n->get_noise()));
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
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

		X_N2[2*i   ] = symbol.real();
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

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::_filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

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
				L0 = MAX(L0, -std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

/*
 * Demodulator
 */
template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::_demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

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
				                        complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * (Q)inv_sigma2);
			else
				L1 = MAX(L1, -std::norm(complex_Yk -
				                        complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
				                                                     (Q)this->constellation[j].imag())) * (Q)inv_sigma2);

		Y_N2[n] = (L0 - L1);
	}
}

template <typename B,typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B,R,Q,MAX>
::_tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

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
			auto tempL = (Q)(std::norm(complex_Yk - std::complex<Q>((Q)this->constellation[j].real(),
			                                                        (Q)this->constellation[j].imag())) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (( (j>>l) & 1 ) == 1)
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
void Modem_QAM<B,R,Q,MAX>
::_tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (!this->n->is_set())
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

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
			                           complex_Hk * std::complex<Q>((Q)this->constellation[j].real(),
			                                                        (Q)this->constellation[j].imag())) * (Q)inv_sigma2);

			for (auto l = 0; l < this->bits_per_symbol; l++)
			{
				if (l == b)
					continue;

				if  (( (j>>l) & 1 ) == 1)
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
/*
* \brief Soft Mapper
*/
template <typename B, typename R, typename Q, tools::proto_max<Q> MAX>
void Modem_QAM<B, R, Q, MAX>
::_tmodulate(const Q *X_N1, R *X_N2, const int frame_id)
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
			std::complex<R> soft_symbol = this->constellation[m];
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
			std::complex<R> soft_symbol = this->constellation[m];
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
}
}
