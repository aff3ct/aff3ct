#include <string>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/AZCW/Encoder_AZCW.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_AZCW<B>
::Encoder_AZCW(const int K, const int N)
: Encoder<B>(K, N)
{
	const std::string name = "Encoder_AZCW";
	this->set_name(name);
}

template <typename B>
Encoder_AZCW<B>* Encoder_AZCW<B>
::clone() const
{
	auto m = new Encoder_AZCW(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_AZCW<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	std::fill(X_N, X_N + this->N, (B)0);
}

template <typename B>
const std::vector<uint32_t>& Encoder_AZCW<B>
::get_info_bits_pos() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
bool Encoder_AZCW<B>
::is_sys() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_AZCW<B_8>;
template class aff3ct::module::Encoder_AZCW<B_16>;
template class aff3ct::module::Encoder_AZCW<B_32>;
template class aff3ct::module::Encoder_AZCW<B_64>;
#else
template class aff3ct::module::Encoder_AZCW<B>;
#endif
// ==================================================================================== explicit template instantiation
