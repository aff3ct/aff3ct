#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Erasure_probability.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename R>
Erasure_probability<R>::Erasure_probability(R noise)
: Noise<R>(noise)
{

}

template <typename R>
template<typename T>
Erasure_probability<R>::
Erasure_probability(const Erasure_probability<T>& other)
: Noise<R>(other)
{
}

template <typename R>
void Erasure_probability<R>::
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
Noise_type Erasure_probability<R>::get_type() const
{
	return Noise_type::EP;
}

template<typename R>
Erasure_probability<R>* Erasure_probability<R>::clone() const
{
	return new Erasure_probability<R>(*this);
}

// ==================================================================================== explicit template instantiation
template class aff3ct::tools::Erasure_probability<float>;
template class aff3ct::tools::Erasure_probability<double>;

template aff3ct::tools::Erasure_probability<double>::Erasure_probability(const Erasure_probability<float >&);
template aff3ct::tools::Erasure_probability<float >::Erasure_probability(const Erasure_probability<double>&);
// ==================================================================================== explicit template instantiation
