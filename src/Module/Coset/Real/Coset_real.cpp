#include <string>

#include "Module/Coset/Real/Coset_real.hpp"

using namespace aff3ct::module;

template <typename B, typename D>
Coset_real<B,D>::Coset_real(const int size)
: Coset<B,D>(size)
{
	const std::string name = "Coset_real";
	this->set_name(name);
}

template <typename B, typename D>
Coset_real<B,D>* Coset_real<B,D>
::clone() const
{
	auto m = new Coset_real(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename D>
void Coset_real<B,D>::_apply(const B *ref, const D *in, D *out, const size_t frame_id)
{
	for (auto i = 0; i < this->size; i++)
		out[i] = ref[i] ? -in[i] : in[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Coset_real<B_8,  Q_8 >;
template class aff3ct::module::Coset_real<B_16, Q_16>;
template class aff3ct::module::Coset_real<B_32, Q_32>;
template class aff3ct::module::Coset_real<B_64, Q_64>;
#else
template class aff3ct::module::Coset_real<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
