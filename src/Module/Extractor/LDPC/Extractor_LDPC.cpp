#include <string>

#include "Module/Extractor/LDPC/Extractor_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Extractor_LDPC<B,Q>
::Extractor_LDPC(const int K, const int N, const std::vector<uint32_t> &info_bits_pos)
: Extractor<B,Q>(K, N, 0),
  info_bits_pos(info_bits_pos)
{
	const std::string name = "Extractor_LDPC";
	this->set_name(name);
}

template <typename B, typename Q>
Extractor_LDPC<B,Q>* Extractor_LDPC<B,Q>
::clone() const
{
	auto m = new Extractor_LDPC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
const std::vector<uint32_t>& Extractor_LDPC<B,Q>
::get_info_bits_pos()
{
	return this->info_bits_pos;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Extractor_LDPC<B_8,Q_8>;
template class aff3ct::module::Extractor_LDPC<B_16,Q_16>;
template class aff3ct::module::Extractor_LDPC<B_32,Q_32>;
template class aff3ct::module::Extractor_LDPC<B_64,Q_64>;
#else
template class aff3ct::module::Extractor_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
