#include <string>

#include "Module/Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"

using namespace aff3ct::module;

template <typename B>
Encoder_RSC3_CPE_sys<B>
::Encoder_RSC3_CPE_sys(const int& K, const int& N, const bool buffered_encoding)
: Encoder_RSC_sys<B>(K, N, 3, buffered_encoding)
{
	const std::string name = "Encoder_RSC3_CPE_sys";
	this->set_name(name);
}

template <typename B>
Encoder_RSC3_CPE_sys<B>* Encoder_RSC3_CPE_sys<B>
::clone() const
{
	auto m = new Encoder_RSC3_CPE_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
int Encoder_RSC3_CPE_sys<B>
::inner_encode(const int bit_sys, int &state)
{
	auto symbol = 0;
	symbol = (bit_sys << 3)              | symbol;
	symbol = (((state >> 2) & 0x1) << 2) | symbol;
	symbol = (((state >> 1) & 0x1) << 1) | symbol;
	symbol = (((state >> 0) & 0x1) << 0) | symbol;

	auto last_bit = state & 0x1;
	state = (bit_sys << 2) | ((state >> 1) ^ last_bit);

	return symbol;
}

template <typename B>
int Encoder_RSC3_CPE_sys<B>
::tail_bit_sys(const int &state)
{
	if(((state & 0x1) ^ ((state >> 1) & 0x1) ^ ((state >> 2) & 0x1)) & 0x1)
		return 1;
	else
		return 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_RSC3_CPE_sys<B_8>;
template class aff3ct::module::Encoder_RSC3_CPE_sys<B_16>;
template class aff3ct::module::Encoder_RSC3_CPE_sys<B_32>;
template class aff3ct::module::Encoder_RSC3_CPE_sys<B_64>;
#else
template class aff3ct::module::Encoder_RSC3_CPE_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
