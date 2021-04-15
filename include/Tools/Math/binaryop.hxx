#include <cmath>     // fabs(), copysign()...
#include <algorithm> // min()

#include "Tools/Math/binaryop.h"

namespace aff3ct
{
namespace tools
{

template <typename TI, typename TO>
inline TO bop_add(const TI a, const TI b)
{
	return (TO)(a + b);
}

template <typename TI, typename TO>
inline TO bop_sub(const TI a, const TI b)
{
	return (TO)(a - b);
}

template <typename TI, typename TO>
inline TO bop_mul(const TI a, const TI b)
{
	return (TO)(a * b);
}

template <typename TI, typename TO>
inline TO bop_div(const TI a, const TI b)
{
	return (TO)(a / b);
}

template <typename TI, typename TO>
inline TO bop_or(const TI a, const TI b)
{
	return (TO)(a | b);
}

template <typename TI, typename TO>
inline TO bop_xor(const TI a, const TI b)
{
	return (TO)(a ^ b);
}

template <typename TI, typename TO>
inline TO bop_and(const TI a, const TI b)
{
	return (TO)(a & b);
}

template <typename TI, typename TO>
inline TO bop_min(const TI a, const TI b)
{
	return (TO)std::min(a, b);
}

template <typename TI, typename TO>
inline TO bop_max(const TI a, const TI b)
{
	return (TO)std::max(a, b);
}

template <typename TI, typename TO, proto_bop<TI,TO> BOP>
std::string bop_get_name()
{
	std::string op = "ukn";
#ifndef _MSC_VER
	     if ((uintptr_t)BOP == (uintptr_t)bop_add<TI,TO>) op = "add";
	else if ((uintptr_t)BOP == (uintptr_t)bop_sub<TI,TO>) op = "sub";
	else if ((uintptr_t)BOP == (uintptr_t)bop_mul<TI,TO>) op = "mul";
	else if ((uintptr_t)BOP == (uintptr_t)bop_div<TI,TO>) op = "div";
	else if ((uintptr_t)BOP == (uintptr_t)bop_or <TI,TO>) op = "or";
	else if ((uintptr_t)BOP == (uintptr_t)bop_xor<TI,TO>) op = "xor";
	else if ((uintptr_t)BOP == (uintptr_t)bop_and<TI,TO>) op = "and";
	else if ((uintptr_t)BOP == (uintptr_t)bop_min<TI,TO>) op = "min";
	else if ((uintptr_t)BOP == (uintptr_t)bop_max<TI,TO>) op = "max";
#endif
	return op;
}

}
}
