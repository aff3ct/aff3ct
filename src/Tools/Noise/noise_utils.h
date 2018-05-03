#ifndef NOISE_UTILS_HPP__
#define NOISE_UTILS_HPP__

#include "Sigma.hpp"
#include "Event_probability.hpp"
#include "Received_optical_power.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T, typename R>
struct Noise_cast
{
	static Noise<T>* cast(const Noise<R>& n)
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
	static Noise<R>* cast(const Noise<R>& n)
	{
		return n.clone();
	}
};

template <typename T, typename R>
Noise<T>* cast(const Noise<R>& n)
{
	return Noise_cast<T,R>::cast(n);
}

}
}
#endif // NOISE_UTILS_HPP__