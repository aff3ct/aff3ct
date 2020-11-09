#include <sstream>
#include <string>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Polar_MK/Encoder_polar_MK_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_polar_MK_sys<B>
::Encoder_polar_MK_sys(const int& K, const int& N, const tools::Polar_code& code, const std::vector<bool>& frozen_bits)
: Encoder_polar_MK<B>(K, N, code, frozen_bits)
{
	const std::string name = "Encoder_polar_MK_sys";
	this->set_name(name);
	this->set_sys(true);

	if (!this->code.can_be_systematic())
	{
		std::stringstream message;
		message << "This polar code does not support systematic encoding.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B>
Encoder_polar_MK_sys<B>* Encoder_polar_MK_sys<B>
::clone() const
{
	auto m = new Encoder_polar_MK_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_polar_MK_sys<B>
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
template class aff3ct::module::Encoder_polar_MK_sys<B_8>;
template class aff3ct::module::Encoder_polar_MK_sys<B_16>;
template class aff3ct::module::Encoder_polar_MK_sys<B_32>;
template class aff3ct::module::Encoder_polar_MK_sys<B_64>;
#else
template class aff3ct::module::Encoder_polar_MK_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
