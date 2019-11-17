#include "Tools/Math/utils.h"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_adaptive.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Scaling_factor_adaptive<B,R>
::Scaling_factor_adaptive(const int n_ite)
: Scaling_factor<B,R>(n_ite)
{
}

template <typename B, typename R>
bool Scaling_factor_adaptive<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	const auto loop_size = (int)ext.size();

	if (ite == 1 || ite == 2) // sf = 0.50
		for (auto i = 0; i < loop_size; i++)
			ext[i] = div2<R>(ext[i]);
/*	else if (ite == this->n_ite) // sf = 1.00
	{}*/
	else // sf = 0.875 instead of 0.85?!
		for (auto i = 0; i < loop_size; i++)
			ext[i] *= (R)0.85; //div4<R>(div2<R>(ext[i])) + div4<R>(div2<R>(ext[i])) + div4<R>(div2<R>(ext[i])) + div2<R>(ext[i]);

	return false;
}

template <typename B, typename R>
Scaling_factor_adaptive<B,R>* Scaling_factor_adaptive<B,R>
::clone() const
{
	auto t = new Scaling_factor_adaptive(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Scaling_factor_adaptive<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	const auto loop_size = (int)ext.size();

	if (ite == 1 || ite == 2) // sf = 0.50
		for (auto i = 0; i < loop_size; i++)
			ext[i] = div2<R>(ext[i]);
/*	else if (ite == this->n_ite) // sf = 1.00
	{}*/
	else // sf = 0.875 instead of 0.85?!
		for (auto i = 0; i < loop_size; i++)
			ext[i] *= (R)0.85; //div4<R>(div2<R>(ext[i])) + div4<R>(div2<R>(ext[i])) + div4<R>(div2<R>(ext[i])) + div2<R>(ext[i]);

	return false;
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Scaling_factor_adaptive<B_8,Q_8>;
template class aff3ct::tools::Scaling_factor_adaptive<B_16,Q_16>;
template class aff3ct::tools::Scaling_factor_adaptive<B_32,Q_32>;
template class aff3ct::tools::Scaling_factor_adaptive<B_64,Q_64>;
#else
template class aff3ct::tools::Scaling_factor_adaptive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
