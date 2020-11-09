#include <string>

#include "Module/Coset/Bit/Coset_bit.hpp"

using namespace aff3ct::module;

template <typename B, typename D>
Coset_bit<B,D>::Coset_bit(const int size)
: Coset<B,D>(size)
{
	const std::string name = "Coset_bit";
	this->set_name(name);
}

template <typename B, typename D>
Coset_bit<B,D>* Coset_bit<B,D>
::clone() const
{
	auto m = new Coset_bit(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename D>
void Coset_bit<B,D>::_apply(const B *ref, const D *in, D *out, const size_t frame_id)
{
	for (auto i = 0; i < this->size; i++)
		out[i] = ref[i] ? !in[i] : in[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Coset_bit<B_8,  B_8 >;
template class aff3ct::module::Coset_bit<B_16, B_16>;
template class aff3ct::module::Coset_bit<B_32, B_32>;
template class aff3ct::module::Coset_bit<B_64, B_64>;
#else
template class aff3ct::module::Coset_bit<B,B>;
#endif
// ==================================================================================== explicit template instantiation
