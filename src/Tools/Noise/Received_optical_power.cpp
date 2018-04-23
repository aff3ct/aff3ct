#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Received_optical_power.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Received_optical_power<R>::Received_optical_power(R noise)
: Noise<R>(noise)
{

}

template<typename R>
Noise_type Received_optical_power<R>::get_type() const
{
	return Noise_type::ROP;
}

//template <typename R>
//Received_optical_power<R>::
//Received_optical_power(const Received_optical_power<R>& other)
//: Noise<R>(other)
//{
//}
//
//template <typename R>
//Received_optical_power<R>::
//Received_optical_power(Received_optical_power<R>&& other)
//: Noise<R>(std::move(other))
//{
//}

template<typename R>
Received_optical_power<R>* Received_optical_power<R>::clone() const
{
	return new Received_optical_power(*this);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Received_optical_power<float>;
template class aff3ct::tools::Received_optical_power<double>;
// ==================================================================================== explicit template instantiation
