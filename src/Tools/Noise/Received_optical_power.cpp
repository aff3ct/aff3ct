#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Received_optical_power.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Received_optical_power<R>
::Received_optical_power(R value)
: Noise<R>(value)
{
	this->check();
}

template<typename R>
Noise_type Received_optical_power<R>
::get_type() const
{
	return Noise_type::ROP;
}

template<typename R>
std::string Received_optical_power<R>
::get_unity() const
{
	return "dB";
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Received_optical_power<float>;
template class aff3ct::tools::Received_optical_power<double>;
// ==================================================================================== explicit template instantiation
