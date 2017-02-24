#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "CRC_polynomial_double.hpp"
using namespace aff3ct;

template <typename B>
CRC_polynomial_double<B>
::CRC_polynomial_double(const int K, std::string poly_key, const int cut_index, const int n_frames, 
                        const std::string name)
: CRC_polynomial<B>(K, poly_key, n_frames, name), cut_index(cut_index)
{
	assert(n_frames == 1);
	assert((this->K - 2 * this->size()) > (cut_index - this->size()));
}

template <typename B>
void CRC_polynomial_double<B>
::build(mipp::vector<B>& U_K)
{
	assert(U_K.size() > (unsigned)(2 * this->size()));
	assert(U_K.size() == (unsigned)this->K);

	for (unsigned i = unsigned(U_K.size() - 2 * this->size() -1); i >= (unsigned)cut_index - this->size(); i--)
		U_K[i + this->size()] = U_K[i];

	this->_generate(U_K, U_K, 0, cut_index - this->size(), cut_index - this->size());
	this->_generate(U_K, U_K, 0, this->K   - this->size(),   this->K - this->size());
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class CRC_polynomial_double<B_8>;
template class CRC_polynomial_double<B_16>;
template class CRC_polynomial_double<B_32>;
template class CRC_polynomial_double<B_64>;
#else
template class CRC_polynomial_double<B>;
#endif
// ==================================================================================== explicit template instantiation
