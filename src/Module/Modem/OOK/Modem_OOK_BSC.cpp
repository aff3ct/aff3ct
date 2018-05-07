#include <type_traits>
#include <cmath>

#include "Tools/Exception/exception.hpp"

#include "Modem_OOK_BSC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK_BSC<B,R,Q>
::Modem_OOK_BSC(const int N, const tools::Noise<R>& noise, const int n_frames)
: Modem_OOK<B,R,Q>(N, noise, n_frames), log_pe_1_pe((Q)0)
{
	const std::string name = "Modem_OOK_BSC";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
void Modem_OOK_BSC<B,R,Q>
::set_noise(const tools::Noise<R>& noise)
{
	Modem_OOK<B,R,Q>::set_noise(noise);

	this->log_pe_1_pe = (Q)log(this->n->get_noise() / (1 - this->n->get_noise())); // trow if noise is not set
}

template <typename B, typename R, typename Q>
void Modem_OOK_BSC<B,R,Q>
::check_noise()
{
	Modem_OOK<B,R,Q>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

template <typename B, typename R, typename Q>
void Modem_OOK_BSC<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	auto log_pe_1_pe_2 = (Q)2 * this->log_pe_1_pe;

	for (auto i = 0; i < this->N_fil; i++)
		Y_N2[i] = log_pe_1_pe_2 * Y_N1[i] - this->log_pe_1_pe; // Y1 = 0 -> Y2 = -log(pe / (1- pe))
		                                                       // Y1 = 1 -> Y2 = +log(pe / (1- pe))
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_OOK_BSC<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_BSC<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_BSC<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_BSC<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_BSC<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_BSC<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_BSC<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_OOK_BSC<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
