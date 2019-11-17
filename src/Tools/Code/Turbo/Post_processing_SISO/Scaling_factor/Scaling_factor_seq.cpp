#include "Tools/Math/utils.h"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_seq.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Scaling_factor_seq<B,R>
::Scaling_factor_seq(const int n_ite)
: Scaling_factor<B,R>(n_ite)
{
}

template <typename B, typename R>
Scaling_factor_seq<B,R>* Scaling_factor_seq<B,R>
::clone() const
{
	auto t = new Scaling_factor_seq(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Scaling_factor_seq<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
{
	const auto loop_size = (int)ext.size();

	// if (ite == 1) // sf = 0.50
	// 	for (auto i = 0; i < loop_size; i++)
	// 		ext[i] = div2<R>(ext[i]);
	// else if (ite == this->n_ite) // sf = 1.00
	// {}
	// else // sf = 0.75
		for (auto i = 0; i < loop_size; i++)
			ext[i] = div2<R>(ext[i]) + div4<R>(ext[i]);

	return false;
}

template <typename B, typename R>
bool Scaling_factor_seq<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	if (ite < this->n_ite -1)
	{
		const auto loop_size = (int)ext.size();

		// if (ite == 1) // sf = 0.50
		// 	for (auto i = 0; i < loop_size; i++)
		// 		ext[i] = div2<R>(ext[i]);
		// else if (ite == this->n_ite) // sf = 1.00
		// {}
		// else // sf = 0.75
			for (auto i = 0; i < loop_size; i++)
				ext[i] = div2<R>(ext[i]) + div4<R>(ext[i]);
	}

	return false;
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Scaling_factor_seq<B_8,Q_8>;
template class aff3ct::tools::Scaling_factor_seq<B_16,Q_16>;
template class aff3ct::tools::Scaling_factor_seq<B_32,Q_32>;
template class aff3ct::tools::Scaling_factor_seq<B_64,Q_64>;
#else
template class aff3ct::tools::Scaling_factor_seq<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
