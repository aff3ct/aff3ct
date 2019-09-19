#include "Tools/Noise/noise_utils.h"
#include "Tools/Perf/distance/Bitwise_diff.h"

namespace aff3ct
{
namespace tools
{

//************************************************************************* any type T and count_unknown_symbols = false
template<typename T, bool cus>
mipp::Reg<T> Bitwise_diff<T,cus>
::apply(const mipp::Reg<T>& in1, const mipp::Reg<T>& in2)
{
	const mipp::Reg<T> zeros = (T)0, ones = (T)1;
	return mipp::blend(ones, zeros, in1 != in2);
}

template<typename T, bool cus>
mipp::Reg<T> Bitwise_diff<T,cus>
::apply(const mipp::Reg<T>& in)
{
	const mipp::Reg<T> zeros = (T)0, ones = (T)1;
	return mipp::blend(ones, zeros, in != zeros);
}

template<typename T, bool cus>
T Bitwise_diff<T,cus>
::apply(const T& in1, const T& in2)
{
	return (in1 != in2) ? (T)1 : (T)0;
}

template<typename T, bool cus>
T Bitwise_diff<T,cus>
::apply(const T& in)
{
	return in != (T)0 ? (T)1 : (T)0;
}

//************************************************************************** any type T and count_unknown_symbols = true
template<typename T>
struct Bitwise_diff<T,true>
{
	static mipp::Reg<T> apply(const mipp::Reg<T>& in1, const mipp::Reg<T>& in2)
	{
		const mipp::Reg<T> zeros  = (T)0, ones = (T)1;
		auto m_in = in1 != in2;
		m_in |= is_unknown_symbol<T>(in1) | is_unknown_symbol<T>(in2);
		return mipp::blend(ones, zeros, m_in);
	}

	static mipp::Reg<T> apply(const mipp::Reg<T>& in)
	{
		const mipp::Reg<T> zeros = (T)0, ones = (T)1;
		const auto m_in = (in != zeros) | is_unknown_symbol<T>(in);
		return mipp::blend(ones, zeros, m_in);
	}

	static T apply(const T& in1, const T& in2)
	{
		return (in1 != in2) || is_unknown_symbol<T>(in1) || is_unknown_symbol<T>(in2) ? (T)1 : (T)0;
	}

	static T apply(const T& in)
	{
		return in != (T)0 || is_unknown_symbol<T>(in) ? (T)1 : (T)0;
	}
};

}
}
