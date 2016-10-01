#include "Coset_bit.hpp"

template <typename B, typename D>
Coset_bit<B,D>::Coset_bit(const int size, const int n_frames, const std::string name)
: Coset<B,D>(size, n_frames, name)
{
}

template <typename B, typename D>
Coset_bit<B,D>::~Coset_bit()
{
}

template <typename B, typename D>
void Coset_bit<B,D>::apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data)
{
	assert(ref.size() == (unsigned)(this->size * this->n_frames));
	assert(ref.size() ==  in_data.size());
	assert(ref.size() == out_data.size());

	for (auto i = 0; i < (int)ref.size(); i++)
		out_data[i] = ref[i] ? !in_data[i] : in_data[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Coset_bit<B_8,  B_8 >;
template class Coset_bit<B_16, B_16>;
template class Coset_bit<B_32, B_32>;
template class Coset_bit<B_64, B_64>;
#else
template class Coset_bit<B,B>;
#endif
// ==================================================================================== explicit template instantiation
