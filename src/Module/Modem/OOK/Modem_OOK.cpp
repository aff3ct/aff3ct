#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Modem_OOK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>
::Modem_OOK(const int N, const R sigma, const bool disable_sig2, const int n_frames)
: Modem<B,R,Q>(N, sigma, n_frames),
  disable_sig2(disable_sig2)
{
	const std::string name = "Modem_OOK";
	this->set_name(name);
	
	if(disable_sig2)
		sigma_factor = (R)0.5;
	else
		sigma_factor = (R)1.0 / (2 * sigma * sigma);
}

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>
::~Modem_OOK()
{
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::set_sigma(const R sigma)
{
	Modem<B,R,Q>::set_sigma(sigma);

	if(disable_sig2)
		sigma_factor = (R)0.5;
	else
		sigma_factor = (R)1.0 / (2 * sigma * sigma);
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::modulate(const B *X_N1, R *X_N2)
{
	auto size = (unsigned int)(this->N * this->n_frames);
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}

template <typename B,typename R, typename Q>
void Modem_OOK<B,R,Q>
::filter(const R *Y_N1, R *Y_N2)
{
	std::copy(Y_N1, Y_N1 + this->N_fil * this->n_frames, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::demodulate(const Q *Y_N1, Q *Y_N2)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = (unsigned int)(this->N_fil * this->n_frames);
	for (unsigned i = 0; i < size; i++)
	{
		Y_N2[i] = -((Q)2.0 * Y_N1[i] - (Q)1) * (Q)sigma_factor;
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	auto size = (unsigned int)(this->N_fil * this->n_frames);
	for (unsigned i = 0; i < size; i++)
	{
		Y_N2[i] = -((Q)2.0 * Y_N1[i] - (Q)1) * (Q)sigma_factor * (Q)H_N[i];
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	this->demodulate(Y_N1,Y_N3);
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
{
	this->demodulate_wg(H_N, Y_N1, Y_N3);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_OOK<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_OOK<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
