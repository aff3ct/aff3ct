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

template<typename R>
template<typename T>
Received_optical_power<R>::
Received_optical_power(const Received_optical_power<T>& other)
: Noise<R>(other)
{
}

template<typename R>
Received_optical_power<R>* Received_optical_power<R>::clone() const
{
	return new Received_optical_power<R>(*this);
}

template<typename R>
std::string Received_optical_power<R>::get_unity() const
{
	return "dB";
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Received_optical_power<float>;
template class aff3ct::tools::Received_optical_power<double>;

template aff3ct::tools::Received_optical_power<double>::Received_optical_power(const Received_optical_power<float >&);
template aff3ct::tools::Received_optical_power<float >::Received_optical_power(const Received_optical_power<double>&);
// ==================================================================================== explicit template instantiation
