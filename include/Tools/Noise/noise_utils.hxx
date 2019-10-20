#include "Tools/Math/utils.h"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Noise/Sigma.hpp"
#include "Tools/Noise/Event_probability.hpp"
#include "Tools/Noise/Received_optical_power.hpp"
#include "Tools/Noise/noise_utils.h"

namespace aff3ct
{
namespace tools
{

template <typename T, typename R>
struct Noise_cast
{
	inline static Noise<T>* cast(const Noise<R>& n)
	{
		Noise<T> * cast_n = nullptr;

		switch(n.get_type())
		{
			case Noise_type::SIGMA: cast_n = new Sigma                 <T>(dynamic_cast<const Sigma                 <R>&>(n)); break;
			case Noise_type::EP:    cast_n = new Event_probability     <T>(dynamic_cast<const Event_probability     <R>&>(n)); break;
			case Noise_type::ROP:   cast_n = new Received_optical_power<T>(dynamic_cast<const Received_optical_power<R>&>(n)); break;
		}

		return cast_n;
	}
};

template <typename R>
struct Noise_cast<R,R>
{
	inline static Noise<R>* cast(const Noise<R>& n)
	{
		return n.clone();
	}
};

template <typename T, typename R>
Noise<T>* cast(const Noise<R>& n)
{
	return Noise_cast<T,R>::cast(n);
}


template <typename R>
constexpr R unknown_symbol_val()
{
	return tools::sat_val<R>();
}

template <typename R>
constexpr R unknown_llr_val()
{
	return (R)1e-5;
}

template <typename R>
bool is_unknown_symbol(const R& v)
{
	return v == tools::unknown_symbol_val<R>();
}

template <typename R>
mipp::Msk<mipp::N<R>()> is_unknown_symbol(const mipp::Reg<R>& q_in)
{
	const mipp::Reg<R> r_unks = tools::unknown_symbol_val<R>();
	return q_in == tools::unknown_symbol_val<R>();
}

template <typename R>
bool is_unknown_llr(const R& v)
{
	return (v <= tools::unknown_llr_val<R>() && v >= -tools::unknown_llr_val<R>());
}

template <typename R>
mipp::Msk<mipp::N<R>()> is_unknown_llr(const mipp::Reg<R>& q_in)
{
	const mipp::Reg<R> r_unk  =  tools::unknown_llr_val<R>();
	const mipp::Reg<R> r_unkm = -tools::unknown_llr_val<R>();
	return (q_in <= r_unk) & (q_in >= r_unkm);
}
}
}
