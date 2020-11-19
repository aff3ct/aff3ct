#include <string>
#include <type_traits>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/OOK/Modem_OOK_AWGN.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK_AWGN<B,R,Q>
::Modem_OOK_AWGN(const int N, const bool disable_sig2)
: Modem_OOK<B,R,Q>(N),
  disable_sig2(disable_sig2),
  sigma_factor((R)1.0)
{
	const std::string name = "Modem_OOK_AWGN";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
Modem_OOK_AWGN<B,R,Q>* Modem_OOK_AWGN<B,R,Q>
::clone() const
{
	auto m = new Modem_OOK_AWGN(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_OOK_AWGN<B,R,Q>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
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

		if (*CP != this->last_channel_param)
			this->sigma_factor = (R)2.0 * (*CP) * (*CP);

		for (auto i = 0; i < this->N_fil; i++)
			Y_N2[i] = -((Q) 2.0 * Y_N1[i] - (Q) 1) * (Q)this->sigma_factor;
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK_AWGN<B,R,Q>
::_demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
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

		if (*CP != this->last_channel_param)
			this->sigma_factor = (R)2.0 * (*CP) * (*CP);

		for (auto i = 0; i < this->N_fil; i++)
			Y_N2[i] = -((Q)2.0 * Y_N1[i] - (Q)1) * (Q)this->sigma_factor * (Q)H_N[i];
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK_AWGN<B,R,Q>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	this->_demodulate(CP, Y_N1, Y_N3, frame_id);
}

template <typename B, typename R, typename Q>
void Modem_OOK_AWGN<B,R,Q>
::_tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	this->_demodulate_wg(CP, H_N, Y_N1, Y_N3, frame_id);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Modem_OOK_AWGN<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_AWGN<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_AWGN<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_AWGN<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_AWGN<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_AWGN<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_AWGN<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_OOK_AWGN<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
