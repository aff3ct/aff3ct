#include <algorithm>

#include "Module/Encoder/NO/Encoder_NO.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_NO<B>
::Encoder_NO(const int K)
: Encoder<B>(K, K)
{
	const std::string name = "Encoder_NO";
	this->set_name(name);
}

template <typename B>
Encoder_NO<B>* Encoder_NO<B>
::clone() const
{
	auto m = new Encoder_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_NO<B>
::_encode(const B *U_K, B *X_K, const size_t frame_id)
{
	std::copy(U_K, U_K + this->K, X_K);
}

template <typename B>
bool Encoder_NO<B>
::is_codeword(const B *X_K)
{
	return true;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_NO<B_8>;
template class aff3ct::module::Encoder_NO<B_16>;
template class aff3ct::module::Encoder_NO<B_32>;
template class aff3ct::module::Encoder_NO<B_64>;
#else
template class aff3ct::module::Encoder_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
