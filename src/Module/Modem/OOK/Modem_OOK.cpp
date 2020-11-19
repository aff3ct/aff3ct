#include <string>
#include <algorithm>
#include <mipp.h>

#include "Module/Modem/OOK/Modem_OOK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>
::Modem_OOK(const int N)
: Modem<B,R,Q>(N)
{
	const std::string name = "Modem_OOK";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
Modem_OOK<B,R,Q>* Modem_OOK<B,R,Q>
::clone() const
{
	auto m = new Modem_OOK(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_OOK<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const size_t frame_id)
{
	for (auto i = 0; i < this->N_fil; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}

namespace aff3ct
{
namespace module
{
template <>
void Modem_OOK<int,float,float>
::_modulate(const int *X_N1, float *X_N2, const size_t frame_id)
{
	using B = int;
	using R = float;

	unsigned size = (unsigned int)(this->N);

	const auto vec_loop_size = (size / mipp::nElReg<B>()) * mipp::nElReg<B>();
	const mipp::Reg<R> Rone  = (R)1.0;
	const mipp::Reg<R> Rzero = (R)0.0;
	const mipp::Reg<B> Bzero = (B)0;

	for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<B>())
	{
		const auto x1b = mipp::Reg<B>(&X_N1[i]);
		const auto x2r = mipp::blend(Rone, Rzero, x1b != Bzero);
		x2r.store(&X_N2[i]);
	}

	for (unsigned i = vec_loop_size; i < size; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}
}
}

template <typename B,typename R, typename Q>
void Modem_OOK<B,R,Q>
::_filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Modem_OOK<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_OOK<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
