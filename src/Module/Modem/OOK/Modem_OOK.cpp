#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Modem_OOK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>
::Modem_OOK(const int N, const tools::Noise<R>& noise, const bool disable_sig2, const int n_frames)
: Modem<B,R,Q>(N, noise, n_frames),
  disable_sig2(disable_sig2),
  sigma_factor((R)0)
{
	const std::string name = "Modem_OOK";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>
::~Modem_OOK()
{
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::set_noise(const tools::Noise<R>& noise)
{
	Modem<B,R,Q>::set_noise(noise);

	this->sigma_factor = (R)2.0 * this->n.get_noise() * this->n.get_noise(); // trow if noise is not set
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	for (auto i = 0; i < this->N_fil; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}

template <typename B,typename R, typename Q>
void Modem_OOK<B,R,Q>
::_filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (disable_sig2)
		for (auto i = 0; i < this->N_fil; i++)
			Y_N2[i] = (Q)0.5 - Y_N1[i];
	else
	{
		if (!std::is_same<R, Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

		if (!std::is_floating_point<Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

		switch (this->n.get_type())
		{
			case tools::Noise_type::SIGMA:
				for (auto i = 0; i < this->N_fil; i++)
					Y_N2[i] = -((Q) 2.0 * Y_N1[i] - (Q) 1) * (Q) sigma_factor;
				break;
			case tools::Noise_type::EP:
				for (auto i = 0; i < this->N_fil; i++)
					Y_N2[i] = Y_N1[i] == std::numeric_limits<Q>::infinity() ? (Q) 0 : ((Q) 1 - (Q) 2.0 * Y_N1[i]);
				break;
			default:
			{
				std::stringstream message;
				message << "The noise has a type other than SIGMA or EP ('this->n.get_type()' = "
				        << this->n.type2str(this->n.get_type()) << ").";
				throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (disable_sig2)
		for (auto i = 0; i < this->N_fil; i++)
			Y_N2[i] = ((Q)0.5 - Y_N1[i]) * (Q)H_N[i];
	else
	{
		if (!std::is_same<R,Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

		if (!std::is_floating_point<Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

		if (!this->n.is_set())
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

		for (auto i = 0; i < this->N_fil; i++)
			Y_N2[i] = -((Q)2.0 * Y_N1[i] - (Q)1) * (Q)sigma_factor * (Q)H_N[i];
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	this->_demodulate(Y_N1,Y_N3,frame_id);
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	this->_demodulate_wg(H_N, Y_N1, Y_N3, frame_id);
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
