#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Event_probability.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Event_probability<R>
::Event_probability(R value)
: Noise<R>(value)
{
	this->check();
}

template <typename R>
void Event_probability<R>
::check()
{
	auto n = this->get_value();
	if (n < (R)-0.00001 || n > (R)1.00001)
	{
		std::stringstream message;
		message << "The EP noise '_n' has to be between [0,1] ('_n' = " << n << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename R>
Noise_type Event_probability<R>
::get_type() const
{
	return Noise_type::EP;
}

template<typename R>
std::string Event_probability<R>
::get_unity() const
{
	return "";
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Event_probability<float>;
template class aff3ct::tools::Event_probability<double>;
// ==================================================================================== explicit template instantiation
