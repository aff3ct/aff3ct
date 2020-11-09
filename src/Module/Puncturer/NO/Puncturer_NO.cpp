#include <string>
#include <algorithm>

#include "Module/Puncturer/NO/Puncturer_NO.hpp"

using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_NO<B,Q>
::Puncturer_NO(const int K, const int N)
: Puncturer<B,Q>(K, N, N)
{
	const std::string name = "Puncturer_NO";
	this->set_name(name);
}

template<typename B, typename Q>
Puncturer_NO<B,Q>* Puncturer_NO<B,Q>
::clone() const
{
	auto m = new Puncturer_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	std::copy(X_N1, X_N1 + this->N, X_N2);
}

template <typename B, typename Q>
void Puncturer_NO<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	std::copy(Y_N1, Y_N1 + this->N, Y_N2);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Puncturer_NO<B_8,Q_8>;
template class aff3ct::module::Puncturer_NO<B_16,Q_16>;
template class aff3ct::module::Puncturer_NO<B_32,Q_32>;
template class aff3ct::module::Puncturer_NO<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_NO<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
