#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Perf/distance/Boolean_diff.h"

//************************************************************************************** common functions for real types

template <typename R>
R boolean_diff_sign(const R in)
{
	return std::signbit(in) ? (R)1 : (R)0;
}

template <typename R>
R boolean_diff_unk_sign(const R in)
{
	return std::signbit(in) || aff3ct::tools::is_unknown_symbol<R>(in) ? (R)1 : (R)0;
}

template <typename R>
R boolean_diff_sign(const R in1, const R in2)
{
	return (std::signbit(in1) ^ std::signbit(in2)) ? (R)1 : (R)0;
}

template <typename R>
R boolean_diff_unk_sign(const R in1, const R in2)
{
	return (std::signbit(in1) ^ std::signbit(in2))
	        || aff3ct::tools::is_unknown_symbol<R>(in1)
	        || aff3ct::tools::is_unknown_symbol<R>(in2)
	       ? (R)1 : (R)0;
}


template <typename R>
mipp::Reg<R> boolean_diff_sign(const mipp::Reg<R>& in)
{
	const mipp::Reg<R> zeros = (R)0, ones = (R)1;
	return mipp::blend(ones, zeros, mipp::sign(in));
}

template <typename R>
mipp::Reg<R> boolean_diff_unk_sign(const mipp::Reg<R>& in)
{
	const mipp::Reg<R> zeros = (R)0, ones = (R)1;
	const auto m_in = mipp::sign(in) | aff3ct::tools::is_unknown_symbol<R>(in);
	return mipp::blend(ones, zeros, m_in);
}

template <typename R>
mipp::Reg<R> boolean_diff_sign(const mipp::Reg<R>& in1, const mipp::Reg<R>& in2)
{
	const mipp::Reg<R> zeros = (R)0, ones = (R)1;
	const auto m_in = mipp::sign(in1) ^ mipp::sign(in2);
	return mipp::blend(ones, zeros, m_in);
}

template <typename R>
mipp::Reg<R> boolean_diff_unk_sign(const mipp::Reg<R>& in1, const mipp::Reg<R>& in2)
{
	const mipp::Reg<R> zeros = (R)0, ones = (R)1;
	const auto m_in = (mipp::sign(in1) ^ mipp::sign(in2))
	                  | aff3ct::tools::is_unknown_symbol<R>(in1)
	                  | aff3ct::tools::is_unknown_symbol<R>(in2);
	return mipp::blend(ones, zeros, m_in);
}




namespace aff3ct
{
namespace tools
{

//******************************************************** type T is integral type (B) and count_unknown_symbols = false
template<typename B, bool cus>
mipp::Reg<B> Boolean_diff<B,cus>
::apply(const mipp::Reg<B>& in1, const mipp::Reg<B>& in2)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	const auto m_in1 = in1 != zeros;
	const auto m_in2 = in2 != zeros;
	return mipp::blend(ones, zeros, m_in1 ^ m_in2);
}

template<typename B, bool cus>
mipp::Reg<B> Boolean_diff<B,cus>
::apply(const mipp::Reg<B>& in)
{
	const mipp::Reg<B> zeros = (B)0, ones = (B)1;
	return mipp::blend(ones, zeros, in != zeros);
}

template<typename B, bool cus>
B Boolean_diff<B,cus>
::apply(const B& in1, const B& in2)
{
	return ((in1 != (B)0) ^ (in2 != (B)0)) ? (B)1 : (B)0;
}

template<typename B, bool cus>
B Boolean_diff<B,cus>
::apply(const B& in)
{
	return in ? (B)1 : (B)0;
}



//********************************************************* type T is integral type (B) and count_unknown_symbols = true
template<typename B>
struct Boolean_diff<B,true>
{
	static mipp::Reg<B> apply(const mipp::Reg<B>& in1, const mipp::Reg<B>& in2)
	{
		const mipp::Reg<B> zeros  = (B)0, ones = (B)1;
		const auto m_in1 = in1 != zeros;
		const auto m_in2 = in2 != zeros;
		const auto m_unk = is_unknown_symbol<B>(in1) | is_unknown_symbol<B>(in2);
		return mipp::blend(ones, zeros, (m_in1 ^ m_in2) | m_unk);
	}

	static mipp::Reg<B> apply(const mipp::Reg<B>& in)
	{
		const mipp::Reg<B> zeros = (B)0, ones = (B)1;
		const auto m_in = (in != zeros) | is_unknown_symbol<B>(in);
		return mipp::blend(ones, zeros, m_in);
	}

	static B apply(const B& in1, const B& in2)
	{
		return ((in1 != (B)0) ^ (in2 != (B)0)) || is_unknown_symbol<B>(in1) || is_unknown_symbol<B>(in2) ? (B)1 : (B)0;
	}

	static B apply(const B& in)
	{
		return in || is_unknown_symbol<B>(in) ? (B)1 : (B)0;
	}
};


//******************************************************* type T is sign type (double) and count_unknown_symbols = false
template<>
struct Boolean_diff<double,false>
{
	static mipp::Reg<double> apply(const mipp::Reg<double>& in1, const mipp::Reg<double>& in2)
	{
		return boolean_diff_sign(in1, in2);
	}

	static mipp::Reg<double> apply(const mipp::Reg<double>& in)
	{
		return boolean_diff_sign(in);
	}

	static double apply(const double& in1, const double& in2)
	{
		return boolean_diff_sign(in1, in2);
	}

	static double apply(const double& in)
	{
		return boolean_diff_sign(in);
	}
};



//******************************************************** type T is sign type (float) and count_unknown_symbols = false
template<>
struct Boolean_diff<float,false>
{
	static mipp::Reg<float> apply(const mipp::Reg<float>& in1, const mipp::Reg<float>& in2)
	{
		return boolean_diff_sign(in1, in2);
	}

	static mipp::Reg<float> apply(const mipp::Reg<float>& in)
	{
		return boolean_diff_sign(in);
	}

	static float apply(const float& in1, const float& in2)
	{
		return boolean_diff_sign(in1, in2);
	}

	static float apply(const float& in)
	{
		return boolean_diff_sign(in);
	}
};



//******************************************************** type T is sign type (double) and count_unknown_symbols = true
template<>
struct Boolean_diff<double,true>
{
	static mipp::Reg<double> apply(const mipp::Reg<double>& in1, const mipp::Reg<double>& in2)
	{
		return boolean_diff_unk_sign(in1, in2);
	}

	static mipp::Reg<double> apply(const mipp::Reg<double>& in)
	{
		return boolean_diff_unk_sign(in);
	}

	static double apply(const double& in1, const double& in2)
	{
		return boolean_diff_unk_sign(in1, in2);
	}

	static double apply(const double& in)
	{
		return boolean_diff_unk_sign(in);
	}
};



//********************************************************* type T is sign type (float) and count_unknown_symbols = true
template<>
struct Boolean_diff<float,true>
{
	static mipp::Reg<float> apply(const mipp::Reg<float>& in1, const mipp::Reg<float>& in2)
	{
		return boolean_diff_unk_sign(in1, in2);
	}

	static mipp::Reg<float> apply(const mipp::Reg<float>& in)
	{
		return boolean_diff_unk_sign(in);
	}

	static float apply(const float& in1, const float& in2)
	{
		return boolean_diff_unk_sign(in1, in2);
	}

	static float apply(const float& in)
	{
		return boolean_diff_unk_sign(in);
	}
};

}
}
