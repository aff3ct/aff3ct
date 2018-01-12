#include "CRC_NO.hpp"

using namespace aff3ct::module;

template <typename B>
CRC_NO<B>
::CRC_NO(const int K, const int n_frames)
: CRC<B>(K, 0, n_frames)
{
	const std::string name = "CRC_NO";
	this->set_name(name);
}

template <typename B>
CRC_NO<B>
::~CRC_NO()
{
}

template <typename B>
int CRC_NO<B>
::get_size() const
{
	return 0;
}

template <typename B>
void CRC_NO<B>
::build(const B *U_K1, B *U_K2)
{
	std::copy(U_K1, U_K1 + this->K * this->n_frames, U_K2);
}

template <typename B>
void CRC_NO<B>
::extract(const B *V_K1, B *V_K2)
{
	std::copy(V_K1, V_K1 + this->K * this->n_frames, V_K2);
}

template <typename B>
bool CRC_NO<B>
::check(const B *V_K, const int n_frames)
{ 
	return false; 
}

template <typename B>
bool CRC_NO<B>
::check_packed(const B *V_K, const int n_frames)
{
	return false;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::CRC_NO<B_8>;
template class aff3ct::module::CRC_NO<B_16>;
template class aff3ct::module::CRC_NO<B_32>;
template class aff3ct::module::CRC_NO<B_64>;
#else
template class aff3ct::module::CRC_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
