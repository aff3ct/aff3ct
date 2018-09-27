#include <type_traits>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Modem_OOK_BEC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK_BEC<B,R,Q>
::Modem_OOK_BEC(const int N, const tools::Noise<R>& noise, const int n_frames)
: Modem_OOK<B,R,Q>(N, noise, n_frames)
{
	const std::string name = "Modem_OOK_BEC";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
void Modem_OOK_BEC<B,R,Q>
::check_noise()
{
	Modem_OOK<B,R,Q>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

template <typename B, typename R, typename Q>
void Modem_OOK_BEC<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	auto sign = tools::unknown_llr_val<Q>();
	for (auto i = 0; i < this->N_fil; i++)
		if (tools::is_unknown_symbol<Q>(Y_N1[i]))
		{
			Y_N2[i] = sign;
			sign *= (Q)-1;
		}
		else
//			Y_N2[i] = std::copysign(tools::sat_val<Q>(), ((Q)1 - (Q)2.0 * Y_N1[i]));
			Y_N2[i] = ((Q)1 - (Q)2.0 * Y_N1[i]);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_OOK_BEC<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_BEC<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_BEC<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_BEC<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_BEC<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_BEC<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_BEC<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_OOK_BEC<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
