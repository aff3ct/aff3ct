#include <cmath> // fabs(), copysign()...

#include "Tools/Math/unaryop.h"

namespace aff3ct
{
namespace tools
{

template <typename TI, typename TO>
inline TO uop_abs(const TI a)
{
	return (TO)std::abs(a);
}

template <typename TI, typename TO>
inline TO uop_not(const TI a)
{
	return (TO)!a;
}

template <typename TI, typename TO>
inline TO uop_not_abs(const TI a)
{
	return (TO)!std::abs(a);
}

template <typename TI, typename TO>
inline TO uop_sign(const TI a)
{
	return (TO)std::signbit(a);
}

template <typename TI, typename TO, proto_uop<TI,TO> UOP>
std::string uop_get_name()
{
	std::string op = "ukn";
#ifndef _MSC_VER
	     if ((uintptr_t)UOP == (uintptr_t)uop_abs    <TI,TO>) op = "abs";
	else if ((uintptr_t)UOP == (uintptr_t)uop_not    <TI,TO>) op = "not";
	else if ((uintptr_t)UOP == (uintptr_t)uop_not_abs<TI,TO>) op = "not_abs";
	else if ((uintptr_t)UOP == (uintptr_t)uop_sign   <TI,TO>) op = "sign";
#endif
	return op;
}

}
}
