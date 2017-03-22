#include "Tools/Math/utils.h"
#include "Tools/Perf/MIPP/mipp.h"

#include "Scaling_factor_vec.hpp"

using namespace aff3ct::tools;

template <typename R>
Scaling_factor_vec<R>
::Scaling_factor_vec(const int K, const int n_ite) 
: Scaling_factor<R>(K, n_ite) 
{
}

template <typename R>
void Scaling_factor_vec<R>
::operator() (mipp::vector<R> &ext, const int ite) 
{
	const auto loop_size1 = (int)(ext.size() / mipp::nElReg<R>());
	const auto loop_size2 = (int)(ext.size());

	// if (ite == 1) // sf = 0.50
	// {
	// 	for (auto i = 0; i < loop_size1; i++)
	// 	{
	//		const auto r_e = mipp::Reg<R>(&ext[i * mipp::nElReg<R>()]);
	//		const auto r_e2 = r_e.div2();
	//		r_e2.store(&ext[i * mipp::nElReg<R>()]);
	// 	}
	//
	// 	for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
	// 		ext[i] = div2<R>(ext[i]); // scaling_factor = 0.5
	// }
	// else if (ite == this->n_ite) // sf = 1.00
	// {}
	// else // sf = 0.75
	// {
		for (auto i = 0; i < loop_size1; i++)
		{
			const auto r_e = mipp::Reg<R>(&ext[i * mipp::nElReg<R>()]);
			const auto r_e2 = r_e.div2() + r_e.div4();
			r_e2.store(&ext[i * mipp::nElReg<R>()]);
		}

		for (auto i = loop_size1 * mipp::nElReg<R>(); i < loop_size2; i++)
			ext[i] = div2<R>(ext[i]) + div4<R>(ext[i]); 
	// }
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::tools::Scaling_factor_vec<Q_8>;
template class aff3ct::tools::Scaling_factor_vec<Q_16>;
template class aff3ct::tools::Scaling_factor_vec<Q_32>;
template class aff3ct::tools::Scaling_factor_vec<Q_64>;
#else
template class aff3ct::tools::Scaling_factor_vec<Q>;
#endif
// ==================================================================================== explicit template instantiation
