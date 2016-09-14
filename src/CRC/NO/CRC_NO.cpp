#include "CRC_NO.hpp"

template <typename B>
CRC_NO<B>
::CRC_NO(const int K, const int n_frames, const std::string name)
: CRC<B>(K, n_frames, name)
{
}

template <typename B>
CRC_NO<B>
::~CRC_NO()
{
}

template <typename B>
int CRC_NO<B>
::size() const 
{
	return 0;
}

template <typename B>
void CRC_NO<B>
::build(mipp::vector<B>& U_K)
{
}

template <typename B>
bool CRC_NO<B>
::check(const mipp::vector<B>& V_K)
{ 
	return false; 
}

// ==================================================================================== explicit template instantiation 
#include "../../Tools/types.h"
#ifdef MULTI_PREC
template class CRC_NO<B_8>;
template class CRC_NO<B_16>;
template class CRC_NO<B_32>;
template class CRC_NO<B_64>;
#else
template class CRC_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
