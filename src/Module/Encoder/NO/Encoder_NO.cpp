#include <stdexcept>

#include "Encoder_NO.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_NO<B>
::Encoder_NO(const int K, const int n_frames)
: Encoder<B>(K, K, n_frames)
{
	const std::string name = "Encoder_NO";
	this->set_name(name);
}

template <typename B>
void Encoder_NO<B>
::_encode(const B *U_K, B *X_K, const int frame_id)
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
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_NO<B_8>;
template class aff3ct::module::Encoder_NO<B_16>;
template class aff3ct::module::Encoder_NO<B_32>;
template class aff3ct::module::Encoder_NO<B_64>;
#else
template class aff3ct::module::Encoder_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
