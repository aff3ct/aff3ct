#include <algorithm>
#include <string>

#include "Module/CRC/NO/CRC_NO.hpp"

using namespace aff3ct::module;

template <typename B>
CRC_NO<B>
::CRC_NO(const int K)
: CRC<B>(K, 0)
{
	const std::string name = "CRC_NO";
	this->set_name(name);
}

template <typename B>
CRC_NO<B>* CRC_NO<B>
::clone() const
{
	auto m = new CRC_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
int CRC_NO<B>
::get_size() const
{
	return 0;
}

template <typename B>
void CRC_NO<B>
::_build(const B *U_K1, B *U_K2, const size_t frame_id)
{
	std::copy(U_K1, U_K1 + this->K, U_K2);
}

template <typename B>
void CRC_NO<B>
::_extract(const B *V_K1, B *V_K2, const size_t frame_id)
{
	std::copy(V_K1, V_K1 + this->K, V_K2);
}

template <typename B>
bool CRC_NO<B>
::_check(const B *V_K, const size_t frame_id)
{
	return false;
}

template <typename B>
bool CRC_NO<B>
::_check_packed(const B *V_K, const size_t frame_id)
{
	return false;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::CRC_NO<B_8>;
template class aff3ct::module::CRC_NO<B_16>;
template class aff3ct::module::CRC_NO<B_32>;
template class aff3ct::module::CRC_NO<B_64>;
#else
template class aff3ct::module::CRC_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
