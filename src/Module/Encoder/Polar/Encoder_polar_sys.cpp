#include <string>

#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_polar_sys<B>
::Encoder_polar_sys(const int& K, const int& N, const std::vector<bool>& frozen_bits)
: Encoder_polar<B>(K, N, frozen_bits)
{
	const std::string name = "Encoder_polar_sys";
	this->set_name(name);
	this->set_sys(true);
}

template <typename B>
Encoder_polar_sys<B>* Encoder_polar_sys<B>
::clone() const
{
	auto m = new Encoder_polar_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_polar_sys<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	this->convert(U_K, X_N);

	// first time encode
	this->light_encode(X_N);

	for (auto i = 0; i < this->N; i++)
		X_N[i] = (B)(!this->frozen_bits[i]) && X_N[i];

	// second time encode because of systematic encoder
	this->light_encode(X_N);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_polar_sys<B_8>;
template class aff3ct::module::Encoder_polar_sys<B_16>;
template class aff3ct::module::Encoder_polar_sys<B_32>;
template class aff3ct::module::Encoder_polar_sys<B_64>;
#else
template class aff3ct::module::Encoder_polar_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
