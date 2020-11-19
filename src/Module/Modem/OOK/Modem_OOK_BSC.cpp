#include <string>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/OOK/Modem_OOK_BSC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK_BSC<B,R,Q>
::Modem_OOK_BSC(const int N)
: Modem_OOK<B,R,Q>(N), log_pe_1_pe((Q)0)
{
	const std::string name = "Modem_OOK_BSC";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
Modem_OOK_BSC<B,R,Q>* Modem_OOK_BSC<B,R,Q>
::clone() const
{
	auto m = new Modem_OOK_BSC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_OOK_BSC<B,R,Q>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (*CP != this->last_channel_param)
	{
		auto proba = *CP;
		if (proba == (R)0.)
			proba = (R)1e-10;
		this->log_pe_1_pe = (Q)log(proba / (1 - proba));
	}

	auto log_pe_1_pe_2 = (Q)2 * this->log_pe_1_pe;

	for (auto i = 0; i < this->N_fil; i++)
		Y_N2[i] = log_pe_1_pe_2 * Y_N1[i] - this->log_pe_1_pe; // Y1 = 0 -> Y2 = -log(pe / (1- pe))
		                                                       // Y1 = 1 -> Y2 = +log(pe / (1- pe))
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Modem_OOK_BSC<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_BSC<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_BSC<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_BSC<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_BSC<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_BSC<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_BSC<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_OOK_BSC<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
