#include <string>
#include <numeric>

#include "Module/Extractor/Uncoded/Extractor_uncoded.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Extractor_uncoded<B,Q>
::Extractor_uncoded(const int K)
: Extractor<B,Q>(K, K, 0),
  info_bits_pos(K, 0)
{
	const std::string name = "Extractor_uncoded";
	this->set_name(name);
	std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
}

template <typename B, typename Q>
Extractor_uncoded<B,Q>* Extractor_uncoded<B,Q>
::clone() const
{
	auto m = new Extractor_uncoded(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
const std::vector<uint32_t>& Extractor_uncoded<B,Q>
::get_info_bits_pos()
{
	return this->info_bits_pos;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Extractor_uncoded<B_8,Q_8>;
template class aff3ct::module::Extractor_uncoded<B_16,Q_16>;
template class aff3ct::module::Extractor_uncoded<B_32,Q_32>;
template class aff3ct::module::Extractor_uncoded<B_64,Q_64>;
#else
template class aff3ct::module::Extractor_uncoded<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
