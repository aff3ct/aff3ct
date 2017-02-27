#include "Coset_real.hpp"

using namespace aff3ct::module;

template <typename B, typename D>
Coset_real<B,D>::Coset_real(const int size, const int n_frames, const std::string name)
: Coset<B,D>(size, n_frames, name)
{
}

template <typename B, typename D>
Coset_real<B,D>::~Coset_real()
{
}

template <typename B, typename D>
void Coset_real<B,D>::apply(const mipp::vector<B>& ref, const mipp::vector<D> &in_data, mipp::vector<D> &out_data)
{
	assert(ref.size() == (unsigned)(this->size * this->n_frames));
	assert(ref.size() ==  in_data.size());
	assert(ref.size() == out_data.size());

	for (auto i = 0; i < (int)ref.size(); i++)
		out_data[i] = ref[i] ? -in_data[i] : in_data[i];
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Coset_real<B_8,  Q_8 >;
template class Coset_real<B_16, Q_16>;
template class Coset_real<B_32, Q_32>;
template class Coset_real<B_64, Q_64>;
#else
template class Coset_real<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
