#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Erased_probability.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Erased_probability<R>::Erased_probability(R noise)
: Noise<R>(noise)
{

}

//template <typename R>
//Erased_probability<R>::
//Erased_probability(const Erased_probability<R>& other)
//: Noise<R>(other)
//{
//}
//
//template <typename R>
//Erased_probability<R>::
//Erased_probability(Erased_probability<R>&& other)
//: Noise<R>(std::move(other))
//{
//}

template <typename R>
void Erased_probability<R>::
check()
{
	auto n = this->get_noise();
	if (n < (R)-0.00001 || n > (R)1.00001)
	{
		std::stringstream message;
		message << "The EP noise '_n' has to be between [0,1] ('_n' = " << n << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename R>
Noise_type Erased_probability<R>::get_type() const
{
	return Noise_type::EP;
}

template<typename R>
Erased_probability<R>* Erased_probability<R>::clone() const
{
	return new Erased_probability(*this);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Erased_probability<float>;
template class aff3ct::tools::Erased_probability<double>;
// ==================================================================================== explicit template instantiation
