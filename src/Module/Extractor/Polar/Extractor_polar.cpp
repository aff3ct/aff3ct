#include <string>

#include "Module/Extractor/Polar/Extractor_polar.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Extractor_polar<B,Q>
::Extractor_polar(const int K, const int N, const std::vector<bool>& frozen_bits)
: Extractor<B,Q>(K, N, 0),
  frozen_bits(frozen_bits), info_bits_pos(K, 0)
{
	const std::string name = "Extractor_polar";
	this->set_name(name);

	this->set_frozen_bits(frozen_bits);
}

template <typename B, typename Q>
Extractor_polar<B,Q>* Extractor_polar<B,Q>
::clone() const
{
	auto m = new Extractor_polar(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Extractor_polar<B,Q>
::set_frozen_bits(const std::vector<bool>& fb)
{
	std::copy(fb.begin(), fb.end(), this->frozen_bits.begin());
	int j = 0;
	for (auto i = 0; i < this->N; i++)
		if (!this->frozen_bits[i])
			this->info_bits_pos[j++] = i;
}

template <typename B, typename Q>
const std::vector<bool>& Extractor_polar<B,Q>
::get_frozen_bits() const
{
	return this->frozen_bits;
}

template <typename B, typename Q>
const std::vector<uint32_t>& Extractor_polar<B,Q>
::get_info_bits_pos()
{
	return this->info_bits_pos;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Extractor_polar<B_8,Q_8>;
template class aff3ct::module::Extractor_polar<B_16,Q_16>;
template class aff3ct::module::Extractor_polar<B_32,Q_32>;
template class aff3ct::module::Extractor_polar<B_64,Q_64>;
#else
template class aff3ct::module::Extractor_polar<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
