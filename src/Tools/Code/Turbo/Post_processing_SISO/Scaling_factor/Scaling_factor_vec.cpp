#include <mipp.h>

#include "Tools/Math/utils.h"
#include "Tools/Code/Turbo/Post_processing_SISO/Scaling_factor/Scaling_factor_vec.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename B, typename R>
Scaling_factor_vec<B,R>
::Scaling_factor_vec(const int n_ite)
: Scaling_factor<B,R>(n_ite)
{
}

template <typename B, typename R>
Scaling_factor_vec<B,R>* Scaling_factor_vec<B,R>
::clone() const
{
	auto t = new Scaling_factor_vec(*this);
	t->deep_copy(*this);
	return t;
}

template <typename B, typename R>
bool Scaling_factor_vec<B,R>
::siso_n(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext,
               mipp::vector<B>& s)
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

	return false;
}

template <typename B, typename R>
bool Scaling_factor_vec<B,R>
::siso_i(const int ite,
         const mipp::vector<R>& sys,
               mipp::vector<R>& ext)
{
	if (ite < this->n_ite -1)
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

	return false;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Scaling_factor_vec<B_8,Q_8>;
template class aff3ct::tools::Scaling_factor_vec<B_16,Q_16>;
template class aff3ct::tools::Scaling_factor_vec<B_32,Q_32>;
template class aff3ct::tools::Scaling_factor_vec<B_64,Q_64>;
#else
template class aff3ct::tools::Scaling_factor_vec<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
