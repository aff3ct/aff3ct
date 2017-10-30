#include <typeinfo>

#include "Tools/Exception/exception.hpp"

#include "Modem_BPSK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_BPSK<B,R,Q>
::Modem_BPSK(const int N, const R sigma, const bool disable_sig2, const int n_frames, const std::string name)
: Modem<B,R,Q>(N, sigma, n_frames, name),
  disable_sig2(disable_sig2), two_on_square_sigma((R)2.0 / (sigma * sigma))
{
}

template <typename B, typename R, typename Q>
Modem_BPSK<B,R,Q>
::~Modem_BPSK()
{
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::set_sigma(const R sigma)
{
	Modem<B,R,Q>::set_sigma(sigma);
	two_on_square_sigma = (R)2.0 / (sigma * sigma);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::modulate(const B *X_N1, R *X_N2)
{
	auto size = (unsigned int)(this->N * this->n_frames);
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = (R)((B)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}

template <typename B,typename R, typename Q>
void Modem_BPSK<B,R,Q>
::filter(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N_fil * this->n_frames, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::demodulate(const Q *Y_N1, Q *Y_N2)
{
	if (disable_sig2)
		std::copy(Y_N1, Y_N1 + this->N * this->n_frames, Y_N2);
	else
	{
		if (typeid(R) != typeid(Q))
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

		if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

		auto size = (unsigned int)(this->N_fil * this->n_frames);
		for (unsigned i = 0; i < size; i++)
		{
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma;
		}
	}

}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2)
{
	if (typeid(R) != typeid(Q))
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (typeid(Q) != typeid(float) && typeid(Q) != typeid(double))
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (disable_sig2)
	{
		auto size = (unsigned int)(this->N_fil * this->n_frames);
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)H_N[i];
	}
	else
	{
		auto size = (unsigned int)(this->N_fil * this->n_frames);
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma * (Q)H_N[i];
	}

}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	this->demodulate(Y_N1,Y_N3);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	this->demodulate_wg(H_N, Y_N1, Y_N3);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_BPSK<B_8,R_8,R_8>;
template class aff3ct::module::Modem_BPSK<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_BPSK<B_16,R_16,R_16>;
template class aff3ct::module::Modem_BPSK<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_BPSK<B_32,R_32,R_32>;
template class aff3ct::module::Modem_BPSK<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_BPSK<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_BPSK<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
