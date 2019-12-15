#include "Tools/Math/utils.h"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_array.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Scaling_factor_array<B,R>
::Scaling_factor_array(const int n_ite, const mipp::vector<float> alpha_array)
: Scaling_factor<B,R>(n_ite), alpha_array(alpha_array)
{
}

template <typename B, typename R>
bool Scaling_factor_array<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	const auto id = (2 * (ite -1) +0) >= (int)alpha_array.size() ? (int)alpha_array.size() -1 : 2 * (ite -1) +0;
	const auto alpha = (R)alpha_array[id];

	const auto loop_size = (int)ext.size();
	for (auto i = 0; i < loop_size; i++)
		ext[i] *= alpha;

	return false;
}

template <typename B, typename R>
Scaling_factor_array<B,R>* Scaling_factor_array<B,R>
::clone() const
{
	auto t = new Scaling_factor_array(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Scaling_factor_array<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	const auto id = (2 * (ite -1) +1) >= (int)alpha_array.size() ? (int)alpha_array.size() -1 : 2 * (ite -1) +1;
	const auto alpha = (R)alpha_array[id];

	const auto loop_size = (int)ext.size();
	for (auto i = 0; i < loop_size; i++)
		ext[i] *= alpha;

	return false;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Scaling_factor_array<B_8,Q_8>;
template class aff3ct::tools::Scaling_factor_array<B_16,Q_16>;
template class aff3ct::tools::Scaling_factor_array<B_32,Q_32>;
template class aff3ct::tools::Scaling_factor_array<B_64,Q_64>;
#else
template class aff3ct::tools::Scaling_factor_array<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
